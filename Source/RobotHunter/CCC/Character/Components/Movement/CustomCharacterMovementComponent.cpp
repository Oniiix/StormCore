#include "CustomCharacterMovementComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "RobotHunter/Utils/FileLogger.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCustomCharacterMovementComponent::InitMovement()
{
	character = Cast<ACharacter>(GetOwner());
	initGravityScale = GravityScale;
	initFallingBrakingDeceleration = BrakingDecelerationFalling;
	initWalkingBrakingDeceleration = BrakingDecelerationWalking;
	initGroundFriction = GroundFriction;
	initMaxAcceleration = MaxAcceleration;
	if (onHandcar)
		handcar->PlayerOverlaps(Cast<AMainCharacter>(character));
}

#pragma region Acceleration
void UCustomCharacterMovementComponent::SetMaxAcceleration(const float _newValue)
{
	MaxAcceleration = _newValue;
}
void UCustomCharacterMovementComponent::NetSetMaxAcceleration(const float _newValue)
{
	MaxAcceleration = _newValue;

	if (OWNER_AUTHORITY)
		ClientRpc_SetMaxAcceleration(_newValue);
	else
		ServerRpc_SetMaxAcceleration(_newValue);
}

void UCustomCharacterMovementComponent::ServerRpc_SetMaxAcceleration_Implementation(const float _value)
{
	MaxAcceleration = _value;
}
void UCustomCharacterMovementComponent::ClientRpc_SetMaxAcceleration_Implementation(const float _value)
{
	MaxAcceleration = _value;
}
bool UCustomCharacterMovementComponent::ClientRpc_SetMaxAcceleration_Validate(const float _value)
{
	return !(OWNER_AUTHORITY);
}

void UCustomCharacterMovementComponent::ResetMaxAcceleration()
{
	MaxAcceleration = initMaxAcceleration;
}
void UCustomCharacterMovementComponent::NetResetMaxacceleration()
{
	MaxAcceleration = initMaxAcceleration;

	if (OWNER_AUTHORITY)
		ClientRpc_SetMaxAcceleration(initMaxAcceleration);
	else
		ServerRpc_SetMaxAcceleration(initMaxAcceleration);
}
#pragma endregion

#pragma region Velocity
void UCustomCharacterMovementComponent::SetVelocity(const FVector& _newVel, const float _gravity)
{
	GravityScale = _gravity;
	Velocity = _newVel;
}
void UCustomCharacterMovementComponent::NetSetVelocity(const FVector& _newVel, const float _gravity)
{
	GravityScale = _gravity;
	Velocity = _newVel;

	if (OWNER_AUTHORITY)
		ClientRpc_SetVelocity(_newVel, _gravity);
	else
		ServerRpc_SetVelocity(_newVel, _gravity);
}


void UCustomCharacterMovementComponent::ServerRpc_SetVelocity_Implementation(const FVector& _value, const float _gravity)
{
	GravityScale = _gravity;
	Velocity = _value;
}

bool UCustomCharacterMovementComponent::ClientRpc_SetVelocity_Validate(const FVector& _value, const float _gravity)
{
	return !(GetOwner()->HasAuthority());
}
void UCustomCharacterMovementComponent::ClientRpc_SetVelocity_Implementation(const FVector& _value, const float _gravity)
{
	GravityScale = _gravity;
	Velocity = _value;
}
#pragma endregion


#pragma region Launch
void UCustomCharacterMovementComponent::LaunchCharacter(const FVector& _force, const float _gravity, const float _brakingDeceleration, const bool _resetOnLanded)
{
	CHECK_NULL(character, "character is null");

	GravityScale = _gravity;
	BrakingDecelerationFalling = _brakingDeceleration;

	character->LaunchCharacter(_force, true, false);

	if (_resetOnLanded)
		character->LandedDelegate.AddUniqueDynamic(this, &UCustomCharacterMovementComponent::OnCharacterLanded);
}
void UCustomCharacterMovementComponent::NetLaunchCharacter(const FVector& _force, const float _gravity, const float _brakingDeceleration, const bool _resetOnLanded)
{
	CHECK_NULL(character, "character is null");

	GravityScale = _gravity; 
	BrakingDecelerationFalling = _brakingDeceleration;

	if (OWNER_AUTHORITY)
		character->LaunchCharacter(_force, true, false);
	else
		ServerRpc_LaunchCharacter(_force, _gravity, _brakingDeceleration, _resetOnLanded);

	if (_resetOnLanded)
		character->LandedDelegate.AddUniqueDynamic(this, &UCustomCharacterMovementComponent::OnCharacterLanded);
}

void UCustomCharacterMovementComponent::ServerRpc_LaunchCharacter_Implementation(const FVector& _force, const float _gravity, const float _brakingDeceleration, const bool _resetOnLanded)
{
	GravityScale = _gravity;
	BrakingDecelerationFalling = _brakingDeceleration;

	character->LaunchCharacter(_force, true, false);

	if (_resetOnLanded)
		character->LandedDelegate.AddUniqueDynamic(this, &UCustomCharacterMovementComponent::OnCharacterLanded);
}


void UCustomCharacterMovementComponent::OnCharacterLanded(const FHitResult& _hit)
{
	GravityScale = initGravityScale;
	BrakingDecelerationFalling = initFallingBrakingDeceleration;
	character->LandedDelegate.RemoveDynamic(this, &UCustomCharacterMovementComponent::OnCharacterLanded);
	onCustomLanded.Broadcast(character);
}
#pragma endregion

#pragma region Gravity
void UCustomCharacterMovementComponent::ResetGravityScale()
{
	GravityScale = initGravityScale;
}
void UCustomCharacterMovementComponent::NetResetGravityScale()
{
	GravityScale = initGravityScale;

	if (OWNER_AUTHORITY)
		ClientRpc_SetGravityScale(initGravityScale);
	else
		ServerRpc_SetGravityScale(initGravityScale);
}


void UCustomCharacterMovementComponent::ServerRpc_SetGravityScale_Implementation(const float _gravityScale)
{
	GravityScale = _gravityScale;
}

bool UCustomCharacterMovementComponent::ClientRpc_SetGravityScale_Validate(const float _gravityScale)
{
	return !(OWNER_AUTHORITY);
}
void UCustomCharacterMovementComponent::ClientRpc_SetGravityScale_Implementation(const float _gravityScale)
{
	GravityScale = _gravityScale;
}
#pragma endregion

#pragma region Friction
void UCustomCharacterMovementComponent::SetFrictionValue(const float _groundFriction, const float _brakingDeceleration)
{
	GroundFriction = _groundFriction;
	BrakingDecelerationWalking = _brakingDeceleration;
}
void UCustomCharacterMovementComponent::NetSetFrictionValue(const float _groundFriction, const float _brakingDeceleration)
{
	GroundFriction = _groundFriction;
	BrakingDecelerationWalking = _brakingDeceleration;

	if (OWNER_AUTHORITY)
		ClientRpc_SetFrictionValue(GroundFriction, BrakingDecelerationWalking);
	else
		ServerRpc_SetFrictionValue(GroundFriction, BrakingDecelerationWalking);
}


void UCustomCharacterMovementComponent::ResetFriction()
{
	GroundFriction = initGroundFriction;
	BrakingDecelerationWalking = initWalkingBrakingDeceleration;
}
void UCustomCharacterMovementComponent::NetResetFriction()
{
	GroundFriction = initGroundFriction;
	BrakingDecelerationWalking = initWalkingBrakingDeceleration;

	if (OWNER_AUTHORITY)
		ClientRpc_SetFrictionValue(GroundFriction, BrakingDecelerationWalking);
	else
		ServerRpc_SetFrictionValue(GroundFriction, BrakingDecelerationWalking);
}



void UCustomCharacterMovementComponent::ServerRpc_SetFrictionValue_Implementation(const float _groundFriction, const float _brakingDeceleration)
{
	GroundFriction = _groundFriction;
	BrakingDecelerationWalking = _brakingDeceleration;
}

bool UCustomCharacterMovementComponent::ClientRpc_SetFrictionValue_Validate(const float _groundFriction, const float _brakingDeceleration)
{
	return !(OWNER_AUTHORITY);
}
void UCustomCharacterMovementComponent::ClientRpc_SetFrictionValue_Implementation(const float _groundFriction, const float _brakingDeceleration)
{
	GroundFriction = _groundFriction;
	BrakingDecelerationWalking = _brakingDeceleration;
}
#pragma endregion


#pragma region override
void UCustomCharacterMovementComponent::UpdateBasedMovement(float DeltaSeconds)
{
	if (!HasValidData())
	{
		return;
	}

	const UPrimitiveComponent* MovementBase = CharacterOwner->GetMovementBase();
	if (!MovementBaseUtility::UseRelativeLocation(MovementBase))
	{
		return;
	}

	if (!IsValid(MovementBase) || !IsValid(MovementBase->GetOwner()))
	{
		SetBase(NULL);
		return;
	}

	// Ignore collision with bases during these movements.
	TGuardValue<EMoveComponentFlags> ScopedFlagRestore(MoveComponentFlags, MoveComponentFlags | MOVECOMP_IgnoreBases);

	FQuat DeltaQuat = FQuat::Identity;
	FVector DeltaPosition = FVector::ZeroVector;

	FQuat NewBaseQuat;
	FVector NewBaseLocation;
	if (!MovementBaseUtility::GetMovementBaseTransform(MovementBase, CharacterOwner->GetBasedMovement().BoneName, NewBaseLocation, NewBaseQuat))
	{
		return;
	}

	// Find change in rotation
	const bool bRotationChanged = !OldBaseQuat.Equals(NewBaseQuat, 1e-8f);
	if (bRotationChanged)
	{
		DeltaQuat = NewBaseQuat * OldBaseQuat.Inverse();
	}

	// only if base moved
	if (bRotationChanged || (OldBaseLocation != NewBaseLocation))
	{
		// Calculate new transform matrix of base actor (ignoring scale).
		const FQuatRotationTranslationMatrix OldLocalToWorld(OldBaseQuat, OldBaseLocation);
		const FQuatRotationTranslationMatrix NewLocalToWorld(NewBaseQuat, NewBaseLocation);

		FQuat FinalQuat = UpdatedComponent->GetComponentQuat();

		if (bRotationChanged && !bIgnoreBaseRotation)
		{
			// Apply change in rotation and pipe through FaceRotation to maintain axis restrictions
			const FQuat PawnOldQuat = UpdatedComponent->GetComponentQuat();
			const FQuat TargetQuat = DeltaQuat * FinalQuat;
			FRotator TargetRotator(TargetQuat);
			CharacterOwner->FaceRotation(TargetRotator, 0.f);
			FinalQuat = UpdatedComponent->GetComponentQuat();

			if (PawnOldQuat.Equals(FinalQuat, 1e-6f))
			{
				// Nothing changed. This means we probably are using another rotation mechanism (bOrientToMovement etc). We should still follow the base object.
				// @todo: This assumes only Yaw is used, currently a valid assumption. This is the only reason FaceRotation() is used above really, aside from being a virtual hook.
				if (bOrientRotationToMovement || (bUseControllerDesiredRotation && CharacterOwner->Controller))
				{
					TargetRotator.Pitch = 0.f;
					TargetRotator.Roll = 0.f;
					MoveUpdatedComponent(FVector::ZeroVector, TargetRotator, false);
					FinalQuat = UpdatedComponent->GetComponentQuat();
				}
			}

			// Pipe through ControlRotation, to affect camera.
			if (CharacterOwner->Controller)
			{
				const FQuat PawnDeltaRotation = FinalQuat * PawnOldQuat.Inverse();
				FRotator FinalRotation = FinalQuat.Rotator();
				UpdateBasedRotation(FinalRotation, PawnDeltaRotation.Rotator());
				FinalQuat = UpdatedComponent->GetComponentQuat();
			}
		}

		// We need to offset the base of the character here, not its origin, so offset by half height
		float HalfHeight, Radius;
		CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleSize(Radius, HalfHeight);

		FVector const BaseOffset(0.0f, 0.0f, HalfHeight);
		FVector const LocalBasePos = OldLocalToWorld.InverseTransformPosition(UpdatedComponent->GetComponentLocation() - BaseOffset);
		FVector const NewWorldPos = ConstrainLocationToPlane(NewLocalToWorld.TransformPosition(LocalBasePos) + BaseOffset);
		DeltaPosition = ConstrainDirectionToPlane(NewWorldPos - UpdatedComponent->GetComponentLocation());

		if (!onHandcar)
		{
			// move attached actor
			if (bFastAttachedMove)
			{
				// we're trusting no other obstacle can prevent the move here
				UpdatedComponent->SetWorldLocationAndRotation(NewWorldPos, FinalQuat, false);
			}
			else
			{
				// Set MovementBase's root actor as ignored when moving the character primitive component, 
				// only perform if bDeferUpdateBasedMovement is true since this means the MovementBase is simulating physics
				const bool bIgnoreBaseActor = bDeferUpdateBasedMovement && bBasedMovementIgnorePhysicsBase;
				AActor* MovementBaseRootActor = nullptr;
				if (bIgnoreBaseActor)
				{
					MovementBaseRootActor = MovementBase->GetAttachmentRootActor();
					UpdatedPrimitive->IgnoreActorWhenMoving(MovementBaseRootActor, true);
					MoveComponentFlags |= MOVECOMP_CheckBlockingRootActorInIgnoreList; // Hit actors during MoveUpdatedComponent will have their root actor compared with the ignored actors array 
				}

				// hack - transforms between local and world space introducing slight error FIXMESTEVE - discuss with engine team: just skip the transforms if no rotation?
				FVector BaseMoveDelta = NewBaseLocation - OldBaseLocation;
				if (!bRotationChanged && (BaseMoveDelta.X == 0.f) && (BaseMoveDelta.Y == 0.f))
				{
					DeltaPosition.X = 0.f;
					DeltaPosition.Y = 0.f;
				}

				FHitResult MoveOnBaseHit(1.f);
				const FVector OldLocation = UpdatedComponent->GetComponentLocation();
				MoveUpdatedComponent(DeltaPosition, FinalQuat, true, &MoveOnBaseHit);

				if ((UpdatedComponent->GetComponentLocation() - (OldLocation + DeltaPosition)).IsNearlyZero() == false)
				{
					OnUnableToFollowBaseMove(DeltaPosition, OldLocation, MoveOnBaseHit);
				}

				// Reset base actor ignore state
				if (bIgnoreBaseActor)
				{
					MoveComponentFlags &= ~MOVECOMP_CheckBlockingRootActorInIgnoreList;
					UpdatedPrimitive->IgnoreActorWhenMoving(MovementBaseRootActor, false);
				}
			}
		}

		if (MovementBase->IsSimulatingPhysics() && GetCharacterOwner()->GetMesh())
		{
			CharacterOwner->GetMesh()->ApplyDeltaToAllPhysicsTransforms(DeltaPosition, DeltaQuat);
		}
	}

	// Check if falling above current base
	if (IsFalling() && bStayBasedInAir)
	{
		const FVector PawnLocation = UpdatedComponent->GetComponentLocation();
		FFindFloorResult OutFloorResult;
		ComputeFloorDist(PawnLocation, StayBasedInAirHeight, StayBasedInAirHeight, OutFloorResult, CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), NULL);

		UPrimitiveComponent* HitComponent = OutFloorResult.HitResult.Component.Get();
		if (!HitComponent || HitComponent->GetAttachmentRoot() != MovementBase->GetAttachmentRoot())
		{
			// New or no base under the character
			ApplyImpartedMovementBaseVelocity();
			SetBase(NULL);
			return;
		}
	}
}
#pragma endregion 
