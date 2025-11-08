// Fill out your copyright notice in the Description page of Project Settings.

#include "NavLinkJumpAI.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

ANavLinkJumpAI::ANavLinkJumpAI()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

void ANavLinkJumpAI::BeginPlay()
{
	Super::BeginPlay();
	OnSmartLinkReached.AddUniqueDynamic(this, &ANavLinkJumpAI::StartJumpAI);
}

void ANavLinkJumpAI::StartJumpAI(AActor* _owner, const FVector& _destinationPoint)
{
	AIOwner = Cast<AAI_Base>(_owner);
	if (!AIOwner)
	{
		LOG_ERROR(NAVLINK_USE_LOG, "Failed to cast owner in character");
		return;
	}

	UAIMovementComponent* _AIMoveComponent = AIOwner->GetAIMovementComponent();
	if (!_AIMoveComponent)
	{
		LOG_ERROR(NAVLINK_USE_LOG, "Failed to get UAIMovementComponent of " + _owner->ACTOR_NAME);
		return;
	}
	/// Stop and lock movement during jump
	_AIMoveComponent->LockMovement();
	AIOwner->GetCharacterMovement()->Velocity = FVector::ZeroVector;

	FVector _outVelocity;
	const FVector& _startLocation = AIOwner->GetActorLocation();
	const bool _isValid = UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, _outVelocity, _startLocation, _destinationPoint, overrideGravity, pushSnapPower);
	if (!_isValid)
	{
		LOG_ERROR(NAVLINK_USE_LOG, "Failed to launch " + _owner->ACTOR_NAME);
		return;
	}

	/// Set orientation and start jump animation
	FRotator _lookAtRotation = UKismetMathLibrary::FindLookAtRotation(_startLocation, _destinationPoint);
	FRotator _ownerRotation = _owner->GetActorRotation();
	_ownerRotation.Yaw = _lookAtRotation.Yaw;
	AIOwner->SetActorRotation(_ownerRotation);

	UAIBaseAnimInstance* _AIAnimInstance = AIOwner->GetAnimInstance();
	if (_AIAnimInstance)
	{
		_AIAnimInstance->Jump();
	}
	else
	{
		LOG_WARNING(NAVLINK_USE_LOG, "Failed to get AI AnimInstance. Abort jump animation");
	}

	FVector _launchVelocity = _outVelocity + FVector(0, 0, 50);
	AIOwner->LaunchCharacter(_launchVelocity * 1.5f, true, true);

	if (!AIOwner->IsPendingKillPending())
		_AIAnimInstance->OnEndJump().AddUniqueDynamic(_AIMoveComponent, &UAIMovementComponent::EndJump);
}
