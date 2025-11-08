// Fill out your copyright notice in the Description page of Project Settings.

#include "AIMovementComponent.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/AI/AI_Base.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RobotHunter/AI/Component/Tracking/TrackingSystemComponent.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "RobotHunter/AI/Arena/Arena.h"

UAIMovementComponent::UAIMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.2f;
}

FVector UAIMovementComponent::GetNewTargetPointRange(const ERangeAttack& _typeRange, const AMainCharacter* _mainCharacter, const bool _canCheckLine, const float _offSetShoot)
{
	rangePositionArena->SetAvailablePoint(targetPoint, true);
	targetPoint = rangePositionArena->GetTargetPoint(_typeRange, owner->GetActorLocation(), _mainCharacter, _canCheckLine);
	return targetPoint.position;
}

void UAIMovementComponent::InitAIMovement()
{
	/// Check owner availability
	owner = Cast<AAI_Base>(GetOwner());
	if (!owner)
	{
		LOG_ERROR(AI_MOVE_COMPONENT_USE_LOG, "Failed to get owner on " + GetFName().ToString());
		return;
	}

	/// Check characterMovementComponent availability 
	characterMovementComponent = owner->GetComponentByClass<UCharacterMovementComponent>();
	if (!characterMovementComponent)
	{
		LOG_ERROR(AI_MOVE_COMPONENT_USE_LOG, "Failed to get CharacterMovementComponent on " + owner->ACTOR_NAME);
		return;
	}

	/// Check BaseAnimInstance availability  
	AIBaseAnimInstance = Cast<UAIBaseAnimInstance>(owner->GetAnimInstance());
	if (!AIBaseAnimInstance)
	{
		LOG_ERROR(TRACKING_SYSTEM_COMPONENT_USE_LOG, "Failed to cast AnimInstance in AIBaseAnimInstance !!");
		return;
	}
	AIBaseAnimInstance->OnEndJump().AddUniqueDynamic(this, &UAIMovementComponent::UnlockMovement);

	/// Check rangePositionArena availability 
	rangePositionArena = owner->GetArena()->GetAIRangePositionComponent();
	if (!rangePositionArena)
	{
		LOG_ERROR(AI_MOVE_COMPONENT_USE_LOG, "Failed to get rangePositionArena on " + owner->ACTOR_NAME);
		return;
	}
	rangePositionArena->OnRefreshPoint().AddUniqueDynamic(this, &UAIMovementComponent::RefreshPoint);

	/// Check controller availability 
	if (owner->HasAuthority())
		controller = owner->GetController<AAIController>();
	if (!controller)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get AAIController on " + owner->ACTOR_NAME);
		return;
	}

	/// Bind on die
	ULifeComponent* _AILifeComponent = owner->GetLifeComponent();
	if (!_AILifeComponent)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get _AILifeComponent on " + owner->ACTOR_NAME);
		return;
	}
	_AILifeComponent->OnDie().AddUniqueDynamic(this, &UAIMovementComponent::ResetPointNav);

	characterMovementComponent->MaxWalkSpeed = walkSpeed;
	characterMovementComponent->MaxAcceleration = acceleration;
	characterMovementComponent->bOrientRotationToMovement = false;

	currentSpeedWalk = walkSpeed;
	currentSpeedRun = runSpeed;
	halfheight = owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.5f;

	drawDebug = owner->GetDrawDebug();
	isInitializationGood = true;
}

void UAIMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isInitializationGood)
		return;

	/// Check if AI is in movement
	//if (controller->GetMoveStatus() == EPathFollowingStatus::Moving)
	//	LOG_COLOR_TIME(true, "MOVING", GREEN, DeltaTime * 1.5f);

	FRotator _newRotation = UpdateRotationToMovement(DeltaTime);
	//_newRotation += UpdateRotationToGround(DeltaTime);
	owner->SetActorRotation(_newRotation);

	if (drawDebug)
		DrawDebug();
}

FRotator UAIMovementComponent::UpdateRotationToGround(float DeltaTime)
{
	FRotator _ownerRotation = owner->GetActorRotation();
	/// Check AIs on ground and moving
	if (characterMovementComponent->IsFalling())
	{
		_ownerRotation.Yaw = 0.f;
		return _ownerRotation;
	}

	const FVector& _ownerLocation = owner->GetActorLocation();
	const FVector& _ownerBottom = _ownerLocation + (FVector::DownVector * halfheight);
	FHitResult _hitResult;
	const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _ownerLocation, _ownerBottom, layersGround, false, {}, EDrawDebugTrace::None, _hitResult, true);
	if (_hit)
	{
		/// Calcul axes
		const FVector& _upVector = _hitResult.ImpactNormal.GetSafeNormal();
		const FVector& _forwardVector = FVector::CrossProduct(_upVector, owner->GetActorRightVector()).GetSafeNormal();
		const FVector& _rightVector = FVector::CrossProduct(_forwardVector, _upVector).GetSafeNormal();

		/// Calcul rotations
		const FRotator& _forwardRotation = FRotationMatrix::MakeFromXZ(_forwardVector, _upVector).Rotator();
		const FRotator& _rightRotation = FRotationMatrix::MakeFromXZ(_rightVector, _upVector).Rotator();
		/// invert Pitch/Roll
		FRotator _targetRotation = _ownerRotation;
		_targetRotation.Pitch = -_forwardRotation.Pitch;
		_targetRotation.Roll = -_rightRotation.Pitch;
		/// Clamp rotation
		const float _maxRotationAngle = 15.f;
		_targetRotation.Pitch = CLAMP(_targetRotation.Pitch, -_maxRotationAngle, _maxRotationAngle);
		_targetRotation.Roll = CLAMP(_targetRotation.Roll, -_maxRotationAngle, _maxRotationAngle);

		/// Lerp rotation
		const float _angularDifference = FMath::Abs(_ownerRotation.Pitch - _targetRotation.Pitch) + FMath::Abs(_ownerRotation.Roll - _targetRotation.Roll);
		const float _minAngleThreshold = 1.f;
		if (_angularDifference > _minAngleThreshold)
		{
			const float _interpSpeed = 100.f;
			FRotator _newRotation = FMath::RInterpConstantTo(_ownerRotation, _targetRotation, DeltaTime, _interpSpeed);
			_newRotation.Yaw = 0.f;
			return _newRotation;
		}
	}

	_ownerRotation.Yaw = 0.f;
	return _ownerRotation;
}

FRotator UAIMovementComponent::UpdateRotationToMovement(float DeltaTime)
{
	const FVector& _ownerVelocity = owner->GetVelocity();
	FRotator _ownerRotation = owner->GetActorRotation();
	/// Check AI is on ground or nearly stoping
	if (characterMovementComponent->IsFalling() || _ownerVelocity.IsNearlyZero(0.1f) || lockingMovement)
	{
		_ownerRotation.Pitch = 0.f;
		_ownerRotation.Roll = 0.f;
		return _ownerRotation;
	}

	/// Find LookAt
	const FVector& _ownerLocation = owner->GetActorLocation();
	const FVector& _movementDirection = _ownerLocation + _ownerVelocity;
	const FRotator& _lookAt = UKismetMathLibrary::FindLookAtRotation(_ownerLocation, _movementDirection);

	/// Lerp rotation
	const float _yawRotation = FMath::FixedTurn(_ownerRotation.Yaw, _lookAt.Yaw, DeltaTime * rotationSpeed * 100);
	FRotator _newRotation = FRotator::ZeroRotator;
	_newRotation.Yaw = _yawRotation;

	/// Update animation
	const float _cross = FVector::CrossProduct(_ownerRotation.Vector(), _lookAt.Vector()).Z;
	const float _rotationAngle = 180.f * _cross;
	AIBaseAnimInstance->SetCurrentAngle(_rotationAngle);

	return _newRotation;
}

void UAIMovementComponent::UpdateSpeed(const bool _value)
{
	if (!isInitializationGood)
		return;
	isRun = _value;
	characterMovementComponent->MaxWalkSpeed = _value ? currentSpeedRun : currentSpeedWalk;
}

void UAIMovementComponent::UpdateAcceleration()
{
	if (!isInitializationGood)
		return;
	characterMovementComponent->MaxAcceleration = acceleration;
}

void UAIMovementComponent::ResetAllSpeed()
{
	ResetRunSpeed();
	ResetWalkSpeed();
}

void UAIMovementComponent::ResetRunSpeed()
{
	currentSpeedRun = runSpeed;
}

void UAIMovementComponent::ResetWalkSpeed()
{
	currentSpeedWalk = walkSpeed;
}

#pragma region Move
void UAIMovementComponent::Move(const FVector& _newLocation, const float _acceptanceRadius)
{
	if (!isInitializationGood || lockingMovement)
		return;

	newLocation = _newLocation;
	controller->MoveToLocation(newLocation, _acceptanceRadius);
	canMove = true;
	OnRepMove();
}

void UAIMovementComponent::Move(AActor* _target, const float _acceptanceRadius)
{
	if (!isInitializationGood || lockingMovement)
		return;

	newLocation = _target->GetActorLocation();
	controller->MoveToActor(_target, _acceptanceRadius);
	canMove = true;
	OnRepMove();
}

void UAIMovementComponent::StopMove()
{
	if (!controller || !isInitializationGood)
		return;

	controller->StopMovement();
	canMove = false;
	OnRepMove();
}

void UAIMovementComponent::OnRepMove()
{
	onMove.Broadcast(canMove);
}

void UAIMovementComponent::EndJump()
{
	Move(targetPoint.position, 50);
}
#pragma endregion

void UAIMovementComponent::LockMovement()
{
	controller->StopMovement();
	lockingMovement = true;
}

void UAIMovementComponent::UnlockMovement()
{
	controller->StopMovement();
	lockingMovement = false;
}

void UAIMovementComponent::LookAtTarget(const FVector& _target)
{
	const FVector& _ownerLocation = owner->GetActorLocation();
	const FRotator& _lookAt = UKismetMathLibrary::FindLookAtRotation(_ownerLocation, _target);
	FRotator _newRotation = owner->GetActorRotation();
	_newRotation.Yaw = _lookAt.Yaw;
	owner->SetActorRotation(_newRotation);
}

void UAIMovementComponent::RefreshPoint()
{
	canMoveInRange = true;
}

void UAIMovementComponent::DrawDebug()
{
	DRAW_SPHERE(newLocation, 50, FColor::Blue);
}

void UAIMovementComponent::ResetPointNav()
{
	rangePositionArena->SetAvailablePoint(targetPoint, true);
}

void UAIMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UAIMovementComponent, canMove);
	//DOREPLIFETIME(UAIMovementComponent, controller);
	//DOREPLIFETIME(UAIMovementComponent, newLocation);
	//DOREPLIFETIME(UAIMovementComponent, lookAt);
}

void UAIMovementComponent::SetSpeedMultiplier(const float _modifier, const float _percent)
{
	const float _percentValue = ((1.0f - _modifier) * (_percent / 100.0f));
	const float _newValue = 1.0f - _percentValue;
	speedMultiplier = _newValue < speedMultiplier ? _newValue : speedMultiplier;
	ModifierSpeed(speedMultiplier);
}

void UAIMovementComponent::ResetSpeedMultiplier()
{
	speedMultiplier = 1.0f;
	ModifierSpeed(speedMultiplier);
}

void UAIMovementComponent::ModifierSpeed(const float _multiplier)
{
	currentSpeedWalk = walkSpeed * _multiplier;
	currentSpeedRun = runSpeed * _multiplier;
	if (!characterMovementComponent)
	{
		if (!owner)
			owner = Cast<AAI_Base>(GetOwner());
		characterMovementComponent = owner->GetComponentByClass<UCharacterMovementComponent>();
	}

	characterMovementComponent->MaxWalkSpeed = isRun ? currentSpeedRun : currentSpeedWalk;
}
