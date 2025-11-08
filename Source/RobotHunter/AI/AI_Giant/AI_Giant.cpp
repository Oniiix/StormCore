// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Giant.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/AI/Settings/TitanSettingsDataAsset.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"

AAI_Giant::AAI_Giant() : AAI_Base(EAIType::Titan)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAI_Giant::InitAI()
{
	Super::InitAI();

	lifeComponent->OnLifeChangeByDamage().AddUniqueDynamic(this, &AAI_Giant::CheckTitanDamage);
	attackComponent->OnAttack().AddUniqueDynamic(this, &AAI_Giant::StartAttack);
	attackComponent->OnEndAttack().AddUniqueDynamic(this, &AAI_Giant::EndAttack);

	GetAnimInstance()->OnEndStun().AddUniqueDynamic(this, &AAI_Giant::EndStun);

	/// Create playerFront
	playerFront = GetWorld()->SpawnActor<ACustomActor>(ACustomActor::StaticClass(), GetActorLocation(), FRotator::ZeroRotator);
	if (!playerFront)
	{
		LOG_ERROR(AI_TITAN_USE_LOG, "Failed to create playerFront");
		return;
	}
	playerFront->SetActorTickEnabled(false);
	/// Check NavigationSystem availability
	navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!navigationSystem)
	{
		LOG_ERROR(AI_TITAN_USE_LOG, "Failed to find navigation system on " + ACTOR_NAME);
		return;
	}

	usingShield = false;
	isInitializationGood = true;
}

void AAI_Giant::SetAI(AArena* _arena)
{
	UTitanSettingsDataAsset* _titanSettings = Cast<UTitanSettingsDataAsset>(settings);
	if (!_titanSettings)
	{
		LOG_ERROR(AI_TITAN_USE_LOG, "Failed to cast settings in Titan' settings");
		return;
	}

	frontLocationModifier = _titanSettings->GetFrontLocationModifier();
	damageThreshold = _titanSettings->GetDamageThreshold();
	delayThreshold = _titanSettings->GetDelayThreshold();
	shieldSpeed = _titanSettings->GetShieldSpeed();
	shieldDisableDelay = _titanSettings->GetShieldDisableDelay();
	frontLayers = _titanSettings->GetFrontLayers();
	if (frontLayers.IsEmpty())
	{
		LOG_ERROR(AI_TITAN_USE_LOG, "frontLayers is empty of " + ACTOR_NAME);
		return;
	}

	Super::SetAI(_arena);
}

FUniqueNetIdRepl AAI_Giant::StartToFight()
{
	FUniqueNetIdRepl _netId = Super::StartToFight();

	player = trackingComponent->GetTarget();
	if (!player)
		LOG_WARNING(AI_TITAN_USE_LOG, "Failed to get target of " + ACTOR_NAME);
	playerCamera = player->GetCameraComponent()->GetCamera();
	if (!playerCamera)
		LOG_WARNING(AI_TITAN_USE_LOG, "Failed to get target of " + ACTOR_NAME);

	return _netId;
}

void AAI_Giant::StunAI(const float _stunTime)
{
	Super::StunAI(_stunTime);

	isStunning = true;

	if (usingShield)
		StopShield();
}

void AAI_Giant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isInitializationGood)
		return;

	const FVector& _updateLocation = UpdatePlayerFront();
	playerFront->SetActorLocation(_updateLocation);
	//DRAW_SPHERE_TIME(_updateLocation, 50.f, FColor::Blue, DeltaTime * 2.f);
}

FVector AAI_Giant::UpdatePlayerFront()
{
	FVector _targetLocation = GetActorLocation();
	if (isFighting && player && playerCamera)
	{
		FVector _playerLocation = player->GetGroundInformation().Location;
		_playerLocation.Z += 50.f;
		_targetLocation = _playerLocation;
		
		FVector _cameraForward = playerCamera->GetForwardVector();
		_cameraForward.Z = 0;
		const float _distanceOwnerToPlayer = FVector::Dist2D(GetActorLocation(), _playerLocation);
		const FVector& _end = _playerLocation + ((_cameraForward * _distanceOwnerToPlayer) * frontLocationModifier);

		FVector _point;
		FHitResult _result;
		const bool _hasHit = UKismetSystemLibrary::LineTraceSingleForObjects(this, _playerLocation, _end, frontLayers, false, { this },
			EDrawDebugTrace::None, _result, true);
		_point = _hasHit ? _result.Location : _end;

		FNavLocation _navLocation;
		bool _isValid = navigationSystem->ProjectPointToNavigation(_point, _navLocation, FVector(100, 100, 100));
		if (_isValid)
			_targetLocation = _navLocation.Location;
	}

	return _targetLocation;
}

void AAI_Giant::CheckTitanDamage(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	if (inAttack || isStunning)
		return;

	if (usingShield)
	{
		/// Restart timer
		GetWorldTimerManager().ClearTimer(timerShieldDelay);
		GetWorld()->GetTimerManager().SetTimer(timerShieldDelay, this, &AAI_Giant::StopShield, shieldDisableDelay);
		return;
	}

	if (!startThreshold)
	{
		startThreshold = true;
		FTimerHandle _timerThreshold;
		GetWorld()->GetTimerManager().SetTimer(_timerThreshold, this, &AAI_Giant::EndDamageThreshold, delayThreshold);
	}
	currentDamage += FMath::Abs(_damage);

	if (currentDamage >= damageThreshold)
		StartShield();
}

void AAI_Giant::SlowAI(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	/// No slowing in shield
	if (usingShield)
		return;

	Super::SlowAI(_owner, _currentLife, _damage, _damageSource);
}

void AAI_Giant::EndDamageThreshold()
{
	startThreshold = false;
	currentDamage = 0;
}

void AAI_Giant::EndStun()
{
	isStunning = false;
}

void AAI_Giant::StartAttack()
{
	inAttack = true;

	if (usingShield)
		StopShield();
}

void AAI_Giant::EndAttack()
{
	inAttack = false;
}

void AAI_Giant::StartShield()
{
	/// Kill slowed status
	statusComponent->RemoveStatus(this, EStatusType::SLOW);

	statusComponent->AddStatus(this, protectStatus);
	onEnableShield.Broadcast();

	usingShield = true;
	movementComponent->SetCurrentSpeedRun(shieldSpeed);
	movementComponent->UpdateSpeed(true);

	GetWorld()->GetTimerManager().SetTimer(timerShieldDelay, this, &AAI_Giant::StopShield, shieldDisableDelay);
}

void AAI_Giant::StopShield()
{
	statusComponent->RemoveStatus(this, EStatusType::PROTECT);
	onDisableShield.Broadcast();

	currentDamage = 0;
	usingShield = false;

	movementComponent->ResetAllSpeed();
	movementComponent->UpdateSpeed(true);
	GetWorldTimerManager().ClearTimer(timerShieldDelay);
}
