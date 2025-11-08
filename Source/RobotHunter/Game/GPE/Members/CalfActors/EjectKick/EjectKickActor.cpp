// Fill out your copyright notice in the Description page of Project Settings.


#include "EjectKickActor.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../../../../../CCC/Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <RobotHunter/AI/AI_Base.h>
#include "Components/CapsuleComponent.h"
#include <RobotHunter/CCC/Character/Animations/CharaAnim.h>
#include "RobotHunter/AI/Component/Life/WeakPointComponent.h"


AEjectKickActor::AEjectKickActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEjectKickActor::InitMember()
{
	Super::InitMember();
	mainCharacter->OnUseCalf().AddUniqueDynamic(this, &AEjectKickActor::EjectJump);
}

void AEjectKickActor::PressBehavior()
{
	Super::PressBehavior();

	FRotator _rotation;

	if (mainCharacter->GetIsAiming() && mainCharacter->GetInputDirection() == FVector2D(0.0f))
		_rotation = FRotator(0.0f, mainCharacter->GetCameraComponent()->GetSpringArm()->GetRelativeRotation().Yaw, 0.0f);
	else
		UpdateRotation(mainCharacter->GetInputDirection(), _rotation);

	mainCharacter->OnRotationEnd().AddUniqueDynamic(this, &AEjectKickActor::SetupKick);

	USpringArmComponent* _arm = mainCharacter->GetCameraComponent()->GetSpringArm();
	if (HasAuthority())
	{
		mainCharacter->SetMeshRotation(_rotation, true, false);
		ClientRpc_StartKick(mainCharacter->GetMesh()->GetRelativeRotation());
	}
	else
	{
		mainCharacter->SetMeshRotation(_rotation, true, false);
		ServerRpc_StartKick(mainCharacter->GetMesh()->GetRelativeRotation());
	}

	mainCharacter->GetCharaAnim()->SetIsScoping(false);
	mainCharacter->SetIsAiming(false);
	mainCharacter->GetCharaAnim()->SetHasStartKick(true);
	mainCharacter->GetCustomMovementComponent()->SetFrictionValue(0.0f, 0.0f);
}

void AEjectKickActor::ReleaseBehavior()
{
	isPressed = false;
}

bool AEjectKickActor::CanUse()
{
	return Super::CanUse() && !isKicking;
}

void AEjectKickActor::ResetMember()
{
	Super::ResetMember();

	mainCharacter->GetCustomMovementComponent()->NetResetGravityScale();
	mainCharacter->GetCustomMovementComponent()->ResetFriction();

	mainCharacter->GetCharaAnim()->SetHasStartKick(false);
	if (isKicking)
	{
		mainCharacter->ReactivateMembers({ this });
		mainCharacter->SetCanMove(true);
		mainCharacter->GetInteractionComponent()->SetCanInteract(true);
		mainCharacter->GetInteractionComponent()->SetCanInteract(true);

		StartCooldown();
	}
	isKicking = false;
	mainCharacter->GetCharaAnim()->SetIsKicking(isKicking);

	mainCharacter->GetCharaAnim()->SetHasReachKick(false);
	//onReachKick.Broadcast(false);
}

void AEjectKickActor::Kick(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	hitComponent->SetCollisionProfileName(FName("IgnoreAll"), false);
	GetWorldTimerManager().ClearTimer(kickTimer);
	LOG_COLOR(true, "Clear Timer", BLUE);

	if (HasAuthority())
		ClientRpc_Hit();
	else
		ServerRpc_Hit();

	mainCharacter->GetCustomMovementComponent()->GravityScale = 0.0f;
	mainCharacter->GetCharacterMovement()->PendingLaunchVelocity = FVector(0.0f);
	mainCharacter->GetCharacterMovement()->Velocity = FVector(0.0f);
	mainCharacter->GetCustomMovementComponent()->SetFrictionValue(0.0f, 0.0f);
	mainCharacter->GetCharaAnim()->SetHasReachKick(true);
	onReachKick.Broadcast(true);

	ULifeComponent* _life = OtherActor->GetComponentByClass<ULifeComponent>();
	if (_life)
	{
		float _damage = damages * bonusDamage;

		if (OverlappedComponent->IsA<UWeakPointComponent>())
			_damage *= Cast<UWeakPointComponent>(OverlappedComponent)->GetDamageMultiplier();

		_life->Damage(_damage, GetMainCharacter());
		AddBleed(OtherActor);
	}
}

void AEjectKickActor::SetupKick()
{
	if (isKicking || !isActivated) return;


	mainCharacter->OnRotationEnd().RemoveDynamic(this, &AEjectKickActor::SetupKick);
	isKicking = true;
	mainCharacter->GetCharaAnim()->SetIsKicking(isKicking);

	if (HasAuthority())
	{
		ClientRpc_Setup();
		mainCharacter->GetCustomMovementComponent()->Velocity = FVector(0.0f);
		mainCharacter->GetCustomMovementComponent()->LaunchCharacter((mainCharacter->GetMesh()->GetForwardVector() * kickForwardForce) + (mainCharacter->GetMesh()->GetUpVector() * kickUpForce));
	}
	else
		ServerRpc_Setup();


	GetWorldTimerManager().SetTimer(kickTimer, this, &AEjectKickActor::EndKick, kickTime);
	LOG_COLOR(true, "Set Timer", BLUE);

	hitComponent->SetCollisionProfileName(FName("OverlapOnlyAI"), true);
}

void AEjectKickActor::EndKick()
{
	hitComponent->SetCollisionProfileName(FName("IgnoreAll"), false);

	mainCharacter->GetCustomMovementComponent()->ResetFriction();


	mainCharacter->GetCharaAnim()->SetHasStartKick(false);

	if (mainCharacter->GetIsOwner())
	{
		isUsing = false;
		mainCharacter->GetInteractionComponent()->SetCanInteract(true);
		StartCooldown();
		mainCharacter->SetCanMove(true);
		mainCharacter->ReactivateMembers({ this });
	}
	isKicking = false;
	mainCharacter->GetCharaAnim()->SetIsKicking(isKicking);
}

void AEjectKickActor::EjectJump()
{
	mainCharacter->GetCustomMovementComponent()->ResetFriction();
	mainCharacter->GetCustomMovementComponent()->NetLaunchCharacter(-mainCharacter->GetMesh()->GetForwardVector() * backDashForce + mainCharacter->GetActorUpVector() * backDashUpForce);
	mainCharacter->GetCharaAnim()->SetHasStartKick(false);

	isKicking = false;
	mainCharacter->GetCharaAnim()->SetIsKicking(isKicking);

	mainCharacter->GetCharaAnim()->SetHasReachKick(false);
	//onReachKick.Broadcast(false);

	if (mainCharacter->GetIsOwner())
	{
		isUsing = false;
		mainCharacter->SetCanMove(true);
		mainCharacter->GetInteractionComponent()->SetCanInteract(true);
		StartCooldown();
		mainCharacter->ReactivateMembers({ this });
	}
}

void AEjectKickActor::StartCooldown()
{
	Super::StartCooldown();
	mainCharacter->OnRotationEnd().RemoveDynamic(this, &AEjectKickActor::SetupKick);
}

void AEjectKickActor::SetupMember()
{
	Super::SetupMember();

	hitComponent = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), FName("HitCapsule"));

	hitComponent->SetupAttachment(GetRootComponent());
	AddInstanceComponent(hitComponent);
	AddOwnedComponent(hitComponent);
	hitComponent->RegisterComponent();

	hitComponent->SetCapsuleSize(capsuleRadius, capsuleHeight, false);

	hitComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEjectKickActor::Kick);
	hitComponent->SetCollisionProfileName(FName("IgnoreAll"), false);

	hitComponent->AttachToComponent(mainCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false), "foot_socket_l");
}


#pragma region Network
void AEjectKickActor::ServerRpc_StartKick_Implementation(const FRotator& _armRotation)
{
	mainCharacter->SetMeshRotation(FRotator(mainCharacter->GetMesh()->GetRelativeRotation().Pitch, _armRotation.Yaw, mainCharacter->GetMesh()->GetRelativeRotation().Roll), true, false);

	mainCharacter->GetCharaAnim()->SetIsScoping(false);
	mainCharacter->GetCharaAnim()->ClearAllAnimWeight(false);
	mainCharacter->SetIsAiming(false);
	mainCharacter->GetCharaAnim()->SetHasStartKick(true);

	mainCharacter->GetCustomMovementComponent()->SetFrictionValue(0.0f, 0.0f);
}

bool AEjectKickActor::ClientRpc_StartKick_Validate (const FRotator& _armRotation)
{
	return !(HasAuthority());
}
void AEjectKickActor::ClientRpc_StartKick_Implementation(const FRotator& _armRotation)
{
	mainCharacter->SetMeshRotation(FRotator(mainCharacter->GetMesh()->GetRelativeRotation().Pitch, _armRotation.Yaw, mainCharacter->GetMesh()->GetRelativeRotation().Roll), true, false);

	mainCharacter->GetCharaAnim()->SetIsScoping(false);
	mainCharacter->GetCharaAnim()->ClearAllAnimWeight(false);
	mainCharacter->SetIsAiming(false);
	mainCharacter->GetCharaAnim()->SetHasStartKick(true);

	mainCharacter->GetCustomMovementComponent()->SetFrictionValue(0.0f, 0.0f);
}


void AEjectKickActor::ServerRpc_Hit_Implementation()
{
	GetWorldTimerManager().ClearTimer(kickTimer);

	mainCharacter->GetCustomMovementComponent()->GravityScale = 0.0f;
	mainCharacter->GetCustomMovementComponent()->Velocity = FVector(0.0f);
	mainCharacter->GetCharaAnim()->SetHasReachKick(true);
}

bool AEjectKickActor::ClientRpc_Hit_Validate()
{
	return !(HasAuthority());
}
void AEjectKickActor::ClientRpc_Hit_Implementation()
{
	GetWorldTimerManager().ClearTimer(kickTimer);

	mainCharacter->GetCustomMovementComponent()->GravityScale = 0.0f;
	mainCharacter->GetCustomMovementComponent()->Velocity = FVector(0.0f);
	mainCharacter->GetCharaAnim()->SetHasReachKick(true);
}


void AEjectKickActor::ServerRpc_Setup_Implementation()
{
	mainCharacter->GetCustomMovementComponent()->Velocity = FVector(0.0f);
	mainCharacter->GetCharaAnim()->SetIsKicking(true);
	GetWorldTimerManager().SetTimer(kickTimer, this, &AEjectKickActor::EndKick, kickTime);
	mainCharacter->LaunchCharacter((mainCharacter->GetMesh()->GetForwardVector() * kickForwardForce) + (mainCharacter->GetMesh()->GetUpVector() * kickUpForce), false, false);
}

bool AEjectKickActor::ClientRpc_Setup_Validate()
{
	return !(HasAuthority());
}
void AEjectKickActor::ClientRpc_Setup_Implementation()
{
	mainCharacter->GetCustomMovementComponent()->Velocity = FVector(0.0f);
	mainCharacter->GetCharaAnim()->SetIsKicking(true);
	GetWorldTimerManager().SetTimer(kickTimer, this, &AEjectKickActor::EndKick, kickTime);
}
#pragma endregion

void AEjectKickActor::UpdateRotation(FVector2D _direction, FRotator& _outRotation)
{
	USkeletalMeshComponent* _mesh = mainCharacter->GetMesh();
	const FRotator& _meshR = _mesh->GetRelativeRotation();

	if (_direction == FVector2D::ZeroVector)
		_outRotation = FRotator(_mesh->GetRelativeRotation().Pitch, _meshR.Yaw, _mesh->GetRelativeRotation().Roll);
	else
	{
		const float _targetYaw = mainCharacter->FindAngle(_direction);
		_outRotation = FRotator(_mesh->GetRelativeRotation().Pitch, _targetYaw, _mesh->GetRelativeRotation().Roll);
	}

}
