// Fill out your copyright notice in the Description page of Project Settings.


#include "STTask_AttackCQC.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/AIsAttackDataAsset.h"

EStateTreeRunStatus USTTask_AttackCQC::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	if (owner->GetCharacterMovement()->IsFalling())
		return EStateTreeRunStatus::Failed;

	/// Get Short Attack
	TArray<FAICQCAttack> _listCQCAttack;
	int _indexCQCAttack;
	arenaCombatComponent->GetShortAttacksForAI(owner->GetType(), _listCQCAttack, _indexCQCAttack);
	const int _listCQCAttackCount = _listCQCAttack.Num();
	if (_listCQCAttackCount == 0)
		return EStateTreeRunStatus::Failed;
	else
	{

		const int _randAttackIndex = RANDOM_INT_RANGE(0, _listCQCAttackCount - 1);
		FAICQCAttack _currentAttackData = _listCQCAttack[_randAttackIndex];
		arenaCombatComponent->GetTokenOnCQCAttack(_indexCQCAttack, _currentAttackData);
		AIAttackComponent->InitCQCAttack(_currentAttackData.CQCAttack);

		return EStateTreeRunStatus::Running;
	}
}
