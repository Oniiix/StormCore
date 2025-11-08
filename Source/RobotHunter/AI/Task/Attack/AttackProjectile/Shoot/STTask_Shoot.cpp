// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_Shoot.h"
#include "RobotHunter/AI/Component/Attack/AttackComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Arena/Component/ArenaCombat/ArenaCombatComponent.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/AIsAttackDataAsset.h"

EStateTreeRunStatus USTTask_Shoot::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

    const TArray<FAIDistanceAttack>& _listDistanceAttack = arenaCombatComponent->GetDistanceAttacksForAI(owner->GetType(), range);
    const int _listDistanceAttackCount = _listDistanceAttack.Num();

    // If no attack available => Move in range
    if (_listDistanceAttackCount == 0)
        return EStateTreeRunStatus::Failed;

    const int _randAttackIndex = RANDOM_INT_RANGE(0, _listDistanceAttackCount - 1);
    const FAIDistanceAttack& _currentAttackData = _listDistanceAttack[_randAttackIndex];

    arenaCombatComponent->GetTokenOnDistanceAttack(range, _randAttackIndex, _currentAttackData);
    AIAttackComponent->AttackDistance(_currentAttackData.projectileRef);
    AIAttackComponent->StartRangeTimer(range);

	movementComponent = owner->GetAIMovementComponent();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTTask_Shoot::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	if (!target)
		return EStateTreeRunStatus();

	const FVector& _targetLocation = target->GetActorLocation();

	//movementComponent->UpdateRotationToMovement(FVector(_targetLocation.X, _targetLocation.Y, owner->GetActorLocation().Z));

	return EStateTreeRunStatus::Running;
}

void USTTask_Shoot::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    Super::ExitState(Context, Transition);
    AIAttackComponent->ClearTimerProjectile();
}
