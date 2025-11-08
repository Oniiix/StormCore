#include "MoveInteractable.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"

AMoveInteractable::AMoveInteractable()
{
	pivotPoint = CreateDefaultSubobject<USceneComponent>("PivotPoint");
	pivotPoint->SetupAttachment(RootComponent);

	targetPoint = CreateDefaultSubobject<USceneComponent>("TargetPoint");
	targetPoint->SetupAttachment(pivotPoint);

	interactionSphere = CreateDefaultSubobject<USphereComponent>("InteractionSphere");
	interactionSphere->SetupAttachment(pivotPoint);

	PrimaryActorTick.bCanEverTick = true;
}

void AMoveInteractable::Interacted(AMainCharacter* _character)
{
	FVector _positionForward = (FVector(targetPoint->GetComponentLocation().X, targetPoint->GetComponentLocation().Y, 0.0f) - FVector(pivotPoint->GetComponentLocation().X, pivotPoint->GetComponentLocation().Y, 0.0f)).GetSafeNormal();
	_positionForward.Z = 0.0f;
	const FVector _locationDiff = (_character->GetActorLocation() - pivotPoint->GetComponentLocation()).GetSafeNormal();
	const float _angle = UCustomMathBlueprintLibrary::Angle(_positionForward, _locationDiff);

	if (_angle > (interactionAngle / 2.0f))
	{
		LOG_COLOR(MOVE_INTERACTABLE_USE_LOG, "Interaction failed: angle condition not met", FColor::Red);
		return;
	}

	player = _character;
	player->DisablePlayerContext(player->GetCurrentPlayerContext());
	player->GetInteractionComponent()->SetHasInteracted(true);
	player->GetCustomMovementComponent()->Velocity = FVector(0.0f);

	movePlayer = true;
	rotatePlayer = true;
}

void AMoveInteractable::Interacted_Internal()
{
	player->EnablePlayerContext(player->GetInputConfig()->GetGameContextInput());
	player->GetInteractionComponent()->SetHasInteracted(false);
	player->SetCanTurn(true);
	Super::Interacted(player);
	player = nullptr;
}

bool AMoveInteractable::CanInteract(AMainCharacter* _character)
{
	return Super::CanInteract(_character) && !player && !_character->GetUsingLeftArm()->GetIsUsing();
}

void AMoveInteractable::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	MovePlayerToPosition();
	RotatePlayerTowardsPosition();
}

void AMoveInteractable::DrawDebug()
{
	Super::DrawDebug();

	DrawDebugSphere(GetWorld(), targetPoint->GetComponentLocation(), 10, 20, FColor::Purple);

	const FVector& _actorLocation = pivotPoint->GetComponentLocation();
	FVector _actorFwd = (FVector(targetPoint->GetComponentLocation().X, targetPoint->GetComponentLocation().Y, 0.0f) - FVector(pivotPoint->GetComponentLocation().X, pivotPoint->GetComponentLocation().Y, 0.0f)).GetSafeNormal();
	const FVector& _actorRight = FVector::CrossProduct(FVector::UpVector, _actorFwd);

	DrawDebugLine(GetWorld(), _actorLocation, _actorLocation + UCustomMathBlueprintLibrary::GetLocalTrigoPointXY(interactionAngle / 2, interactionSphere->GetUnscaledSphereRadius(), _actorFwd, _actorRight), FColor::Blue);
	DrawDebugLine(GetWorld(), _actorLocation, _actorLocation + UCustomMathBlueprintLibrary::GetLocalTrigoPointXY(-(interactionAngle / 2), interactionSphere->GetUnscaledSphereRadius(), _actorFwd, _actorRight), FColor::Blue);
}

void AMoveInteractable::MovePlayerToPosition()
{
	if (movePlayer && player)
	{
		const FVector& _targetPointLoc = targetPoint->GetComponentLocation();
		const FVector& _playerLoc = player->GetActorLocation();

		const float _distance = FVector2D::Distance(FVector2D(_targetPointLoc), FVector2D(_playerLoc));

		if (_distance <= tolerance)
		{
			player->GetCustomMovementComponent()->Velocity = FVector(0.f);
			movePlayer = false;
			const FVector& _pivotPointLoc = pivotPoint->GetComponentLocation();
			targetRotation = UKismetMathLibrary::FindLookAtRotation(_playerLoc, FVector(_pivotPointLoc.X, _pivotPointLoc.Y, _playerLoc.Z));
			return;
		}

		const FVector& _meshForward = player->GetMesh()->GetForwardVector() * 0.5f;
		player->AddMovementInput(_meshForward);
	}
}

void AMoveInteractable::RotatePlayerTowardsPosition()
{
	if (rotatePlayer  && player)
	{
		const FVector& _pivotPointLoc = pivotPoint->GetComponentLocation();
		const FVector& _targetPointLoc = targetPoint->GetComponentLocation();
		const FVector& _playerLoc = player->GetActorLocation();

		FRotator _rotation = movePlayer ? UKismetMathLibrary::FindLookAtRotation(_playerLoc, FVector(_targetPointLoc.X, _targetPointLoc.Y, _playerLoc.Z))
			: targetRotation;

		if (!movePlayer && UCustomMathBlueprintLibrary::IsFloatEqual(player->GetMesh()->GetComponentRotation().Yaw, _rotation.Yaw, 1.0f))
		{
			rotatePlayer = false;
			Interacted_Internal();
			return;
		}

		player->SetMeshRotation(_rotation, !movePlayer);
	}
}
