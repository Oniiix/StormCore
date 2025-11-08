
#include "STTask_MoveToPlayerFront.h"
#include "RobotHunter/AI/AI_Giant/AI_Giant.h"

EStateTreeRunStatus USTTask_MoveToPlayerFront::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	player = AITrackingComponent->GetTarget();
	if (!player)
	{
		LOG_ERROR(TASK_MOVE_FRONT_USE_LOG, "Failed to get target of " + owner->ACTOR_NAME);
		return EStateTreeRunStatus::Failed;
	}

	titanOwner = Cast<AAI_Giant>(owner);
	if (!titanOwner)
	{
		LOG_ERROR(TASK_MOVE_FRONT_USE_LOG, "Failed to cast " + owner->ACTOR_NAME + " in AI_Titan");
		return EStateTreeRunStatus::Failed;
	}

	const FVector& _ownerLocation = owner->GetActorLocation();
	const FVector& _playerLocation = player->GetActorLocation();
	const float _distPlayerToOwner = FVector::Dist2D(_playerLocation, _ownerLocation);

	if (!owner->WasRecentlyRendered())
	{
		target = player;
		movementComponent->Move(target, acceptanceRadius);
	}
	else
	{
		target = titanOwner->GetPlayerFront();
		movementComponent->Move(target, 50.f);
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_MoveToPlayerFront::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	/// Unlock if didn't move
	const float _ownerVelocity = owner->GetVelocity().Length();
	if (_ownerVelocity <= 1.f)
	{
		countDeltaTime += DeltaTime;
		if (countDeltaTime >= 0.1f)
			return EStateTreeRunStatus::Failed;
	}

	const FVector& _ownerLocation = owner->GetActorLocation();
	const FVector& _playerLocation = player->GetActorLocation();
	const float _distPlayerToOwner = FVector::Dist2D(_playerLocation, _ownerLocation);

	if (_distPlayerToOwner <= acceptanceRadius)
		return EStateTreeRunStatus::Failed;

	if (!owner->WasRecentlyRendered(DeltaTime) && target != player)
	{
		target = player;
		movementComponent->Move(target, acceptanceRadius);
	}
	else if (owner->WasRecentlyRendered(DeltaTime) && target == player)
	{
		target = titanOwner->GetPlayerFront();
		movementComponent->Move(target, 50.f);
	}

	return EStateTreeRunStatus::Running;
}
