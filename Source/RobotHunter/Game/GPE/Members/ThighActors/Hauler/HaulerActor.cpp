// Fill out your copyright notice in the Description page of Project Settings.


#include "HaulerActor.h"
#include "../../../../../CCC/Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AHaulerActor::AHaulerActor()
{
	distance = maxSpeed * totalDashTime;
}

void AHaulerActor::BeginPlay()
{
	Super::BeginPlay();

}

void AHaulerActor::PressBehavior()
{
	Super::PressBehavior();
	mainCharacter->DeactivateMembers({ this });

	onUseHauler.Broadcast();
	wasAiming = mainCharacter->GetIsAiming();
	mainCharacter->GetInteractionComponent()->SetCanInteract(false);

	//�a dans le Reset
	currentTime = 0;

	// �a c'est bon
	canDash = false;

	if (wasAiming && mainCharacter->GetInputDirection() == FVector2D(0.0f))
		rotation = FRotator(0.0f, mainCharacter->GetCameraComponent()->GetSpringArm()->GetRelativeRotation().Yaw, 0.0f);
	else
		UpdateRotation(mainCharacter->GetInputDirection());

	mainCharacter->SetMeshRotation(rotation, false, false);
	mainCharacter->SetIsDashing(true);
	mainCharacter->GetCustomMovementComponent()->NetSetFrictionValue(0.0f, 0.0f);
	GetWorld()->GetTimerManager().SetTimer(dashTimer, this, &AHaulerActor::EndDash, totalDashTime);
}

bool AHaulerActor::CanUse()
{
	return Super::CanUse() && canDash && (currentNbAirDash < nbAirDash);
}

void AHaulerActor::Dash()
{
	if (!canShot)return;
	if (HasAuthority())
	{
		velocity = mainCharacter->GetMesh()->GetForwardVector() * currentSpeed;
		mainCharacter->GetCharacterMovement()->Velocity = velocity;
	}
	else
	{
		velocity = mainCharacter->GetMesh()->GetForwardVector() * currentSpeed;
		mainCharacter->GetCharacterMovement()->Velocity = velocity;
		ServerRpc_SetClientVelocity(mainCharacter->GetMesh()->GetForwardVector() * currentSpeed);
	}
}

void AHaulerActor::ReleaseBehavior()
{
	isPressed = false;
}

void AHaulerActor::ResetMember()
{
	Super::ResetMember();
	if (!canDash)
	{
		canDash = true;
		mainCharacter->SetCanTurn(true);
		hasStarted = false;
		mainCharacter->ReactivateMembers({ this });
		mainCharacter->GetInteractionComponent()->SetCanInteract(true);
		mainCharacter->ActivateScope();
		StartCooldown();
	}
	mainCharacter->SetIsDashing(false);
	mainCharacter->GetCustomMovementComponent()->NetResetFriction();
	GetWorld()->GetTimerManager().ClearTimer(dashTimer);
}

void AHaulerActor::InitMember()
{
	mainCharacter->OnMove().AddUniqueDynamic(this, &AHaulerActor::UpdateRotation);
	mainCharacter->OnRotationEnd().AddUniqueDynamic(this, &AHaulerActor::StartDash);
}

void AHaulerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (canShot && !canDash)
	{
		currentTime += DeltaTime;
		UpdateSpeed();
		Dash();
	}
}

void AHaulerActor::MultiRpc_SetVelocity_Implementation(FVector _velocity)
{
	velocity = _velocity;
	mainCharacter->GetCharacterMovement()->Velocity = velocity;

}
void AHaulerActor::ServerRpc_SetClientVelocity_Implementation(FVector _velocity)
{
	LOG_COLOR(HAULER_USE_LOG, velocity.ToString(), PINK);
	MultiRpc_SetVelocity_Implementation(_velocity);
}

void AHaulerActor::UpdateRotation(FVector2D _direction)
{
	USkeletalMeshComponent* _mesh = mainCharacter->GetMesh();
	const FRotator& _meshR = _mesh->GetRelativeRotation();

	if (_direction == FVector2D::ZeroVector)
		rotation = FRotator(_mesh->GetRelativeRotation().Pitch, _meshR.Yaw, _mesh->GetRelativeRotation().Roll);
	else
	{
		const float _targetYaw = mainCharacter->FindAngle(_direction);
		rotation = FRotator(_mesh->GetRelativeRotation().Pitch, _targetYaw, _mesh->GetRelativeRotation().Roll);
	}

}

void AHaulerActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHaulerActor, velocity);
	DOREPLIFETIME(AHaulerActor, rotation);
}


void AHaulerActor::UpdateSpeed()
{
	currentSpeed = speedCurve->GetFloatValue(currentTime / totalDashTime) * maxSpeed;
}

void AHaulerActor::EndDash()
{
	canDash = true;
	mainCharacter->SetIsDashing(false);
	mainCharacter->SetCanTurn(true);
	hasStarted = false;
	mainCharacter->GetCustomMovementComponent()->NetResetFriction();
	mainCharacter->ReactivateMembers({this});
	mainCharacter->GetInteractionComponent()->SetCanInteract(true);
	mainCharacter->ActivateScope();


	GetWorld()->GetTimerManager().ClearTimer(dashTimer);
	if (HasAuthority())
	{
		velocity = mainCharacter->GetCharacterMovement()->Velocity * (mainCharacter->GetCustomMovementComponent()->IsFalling() ? fallVelocityReduceMultiplier : groundVelocityReduceMultiplier);
		mainCharacter->GetCharacterMovement()->Velocity = velocity;
	}
	else
		ServerRpc_SetClientVelocity(mainCharacter->GetCharacterMovement()->Velocity * (mainCharacter->GetCustomMovementComponent()->IsFalling() ? fallVelocityReduceMultiplier : groundVelocityReduceMultiplier));

	StartCooldown();
	
	//mainCharacter->EnablePlayerContext(mainCharacter->GetInputConfig()->GetGameContextInput());

	currentNbAirDash = !mainCharacter->GetCustomMovementComponent()->IsFalling() ? 0 : currentNbAirDash + 1;
	if (currentNbAirDash == 1)
		mainCharacter->LandedDelegate.AddUniqueDynamic(this, &AHaulerActor::TouchGround);
}

void AHaulerActor::StartDash()
{
	if (!canDash&& !hasStarted)
	{
		mainCharacter->SetIsDashing(true);
		hasStarted = true;
		mainCharacter->SetCanTurn(false);
		mainCharacter->DisablePlayerContext(mainCharacter->GetCurrentPlayerContext());
	}
	
}

bool AHaulerActor::CanDash()
{
	if (!canDash || !canShot || mainCharacter->GetCustomMovementComponent()->IsFalling() && (currentNbAirDash == nbAirDash))
		return false;

	
	return true;
}

void AHaulerActor::TouchGround(const FHitResult& _result)
{
	currentNbAirDash = 0;
	mainCharacter->LandedDelegate.RemoveDynamic(this, &AHaulerActor::TouchGround);
}

#if WITH_EDITOR
void AHaulerActor::PostEditChangeProperty(FPropertyChangedEvent& _property)
{
	Super::PostEditChangeProperty(_property);
	const FString _propertyName = _property.GetPropertyName().ToString();
	if (_propertyName == GET_MEMBER_NAME_CHECKED(AHaulerActor, maxSpeed) || _propertyName == GET_MEMBER_NAME_CHECKED(AHaulerActor, totalDashTime))
	{
		distance = maxSpeed * totalDashTime;
	}
}
#endif 
