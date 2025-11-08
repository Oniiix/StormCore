// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackCQCDataAsset.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"

void UAttackCQCDataAsset::CQCEffect(AActor* _target, AActor* _owner)
{
	if (!_target || !_owner)
	{
		LOG_ERROR(USE_LOG_ATTACK_CQC, "Target and/or owner is null");
		return;
	}

	switch (effect)
	{
	case ECQCEffect::Nothing:
		/// nth
		LOG_COLOR(USE_LOG_ATTACK_CQC, "Effect : nothing", PINK);
		break;
	case ECQCEffect::PushTarget:
		LOG_COLOR(USE_LOG_ATTACK_CQC, "Effect : push target", PINK);
		PushTarget(_target, _owner);
		break;
	case ECQCEffect::PushOwner:
		LOG_COLOR(USE_LOG_ATTACK_CQC, "Effect : push owner", PINK);
		PushOwner(_target, _owner);
		break;
	}
}

void UAttackCQCDataAsset::PushTarget(AActor* _target, AActor* _owner)
{
	ACharacter* _charaTarget = Cast<ACharacter>(_target);
	if (!_charaTarget)
	{
		LOG_ERROR(USE_LOG_ATTACK_CQC, "Failed to cast target in ACharacter => Can't push target");
		return;
	}
	/// Stop target movement
	_charaTarget->GetController()->StopMovement();
	_charaTarget->GetCharacterMovement()->Velocity = FVector::ZeroVector;

	/// Calcul direction and launch target
	const FVector& _charaLocation = _charaTarget->GetActorLocation();
	const FVector& _targetDirection = _charaLocation - _owner->GetActorLocation();
	const FVector& _targetLocation = _charaLocation + ((_targetDirection.GetSafeNormal2D()) * pushDistance);
	FVector _outLaunchVelocity;


	AMainCharacter* _mainCharaTarget = Cast<AMainCharacter>(_charaTarget);
	if (_mainCharaTarget)
	{
		_mainCharaTarget->BumpPlayer(_owner->GetActorLocation());
	}

	const bool _isValid = UGameplayStatics::SuggestProjectileVelocity_CustomArc(_target->GetWorld(), _outLaunchVelocity, _charaLocation, _targetLocation, 0, pushSnapPower);
	if (!_isValid)
	{
		LOG_WARNING(USE_LOG_ATTACK_CQC, "Failed to properly launch target");
		_charaTarget->LaunchCharacter(_targetLocation, true, true);
		if (drawDebug)
			DrawDebugSphere(_target->GetWorld(), _targetLocation, 50.f, 30, FColor::Purple, false, 3);
	}
	else
	{
		_charaTarget->LaunchCharacter(_outLaunchVelocity, true, true);
		if (drawDebug)
			DrawDebugSphere(_target->GetWorld(), _targetLocation, 50.f, 30, FColor::Purple, false, 3);
	}
}

void UAttackCQCDataAsset::PushOwner(AActor* _target, AActor* _owner)
{
	ACharacter* _charaOwner = Cast<ACharacter>(_owner);
	if (!_charaOwner)
	{
		LOG_ERROR(USE_LOG_ATTACK_CQC, "Failed to cast owner in ACharacter => Can't push owner");
		return;
	}
	/// Stop owner movement
	_charaOwner->GetController()->StopMovement();
	_charaOwner->GetCharacterMovement()->Velocity = FVector::ZeroVector;

	/// Calcul direction and launch target
	const FVector& _charaLocation = _charaOwner->GetActorLocation();
	const FVector& _targetDirection = _charaLocation - _target->GetActorLocation();
	const FVector& _targetLocation = _charaLocation + ((_targetDirection.GetSafeNormal2D()) * pushDistance);
	FVector _outLaunchVelocity;

	const bool _isValid = UGameplayStatics::SuggestProjectileVelocity_CustomArc(_owner->GetWorld(), _outLaunchVelocity, _charaLocation, _targetLocation, 0, pushSnapPower);
	if (!_isValid)
	{
		LOG_WARNING(USE_LOG_ATTACK_CQC, "Failed to properly launch owner");
		_charaOwner->LaunchCharacter(_targetLocation, true, true);
		if (drawDebug)
			DrawDebugSphere(_target->GetWorld(), _targetLocation, 50.f, 30, FColor::Purple, false, 3);
	}
	else
	{
		_charaOwner->LaunchCharacter(_outLaunchVelocity, false, false);
		if (drawDebug)
			DrawDebugSphere(_target->GetWorld(), _targetLocation, 50.f, 30, FColor::Purple, false, 3);
	}
}

