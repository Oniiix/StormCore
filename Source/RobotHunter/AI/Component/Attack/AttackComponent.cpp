// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackComponent.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/ArenaManager.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Attack/CQC/AttackCQCDataAsset.h"
#include "RobotHunter/AI/Attack/Projectile/Projectile.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/ProjectileStats.h"
#include "ThreatReasonDataAsset.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "GameFramework/PlayerState.h"
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"
#include "Net/UnrealNetwork.h"
#include "RobotHunter/AI/Settings/AIsSettingsDataAsset.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "RobotHunter/AI/Component/Tracking/TrackingSystemComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = CUSTOM_TICK;

	disableAttack = false;
	currentNumberOfShoot = 0;
	isInitializationGood = false;
}

void UAttackComponent::SetListRange(const TArray<FRangeAttack>& _listRange)
{
	const int _listRangeCount = _listRange.Num();
	for (int i = 0; i < _listRangeCount; i++)
	{
		FAIRangeAttack _AIRangeAttack = FAIRangeAttack(_listRange[i].cooldown);
		mapRange.Add(_listRange[i].range, _AIRangeAttack);
	}
}

void UAttackComponent::InitAttack()
{
	/// Check owner availability
	owner = Cast<AAI_Base>(GetOwner());
	if (!owner)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to cast owner in AAI_Base");
		return;
	}

	/// Check animInstance availability
	animInstance = Cast<UAIBaseAnimInstance>(owner->GetMesh()->AnimScriptInstance);
	if (!animInstance)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to cast animInstance of " + owner->ACTOR_NAME + " on UAIBaseAnimInstance");
		return;
	}
	animInstance->OnEnableHitBox().AddUniqueDynamic(this, &UAttackComponent::EnableHitBox);
	animInstance->OnDisableHitBox().AddUniqueDynamic(this, &UAttackComponent::DisableHitBox);
	animInstance->OnJumpAttack().AddUniqueDynamic(this, &UAttackComponent::JumpAttack);

	/// Check UAIMovementComponent availability
	AIMovementComponent = owner->GetAIMovementComponent();
	if (!AIMovementComponent)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get UAIMovementComponent on " + owner->ACTOR_NAME);
		return;
	}

	/// Check UMovableComponent availability
	movableComponent = owner->GetMovableComponent();
	if (!movableComponent)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get UMovableComponent on " + owner->ACTOR_NAME);
		return;
	}
	movableComponent->OnLaunch().AddUniqueDynamic(this, &UAttackComponent::ResetAttack);

	/// Check trackingSystem availability
	trackingSystem = owner->GetTrackingSystemComponent();
	if (!trackingSystem)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get trackingSystem on " + owner->ACTOR_NAME);
		return;
	}
	trackingSystem->OnEndLookAtTarget().AddUniqueDynamic(this, &UAttackComponent::StartAttack);

	// Check AIController availability
	AIController = owner->GetController<AAIController>();
	if (!AIController)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get AIController on " + owner->ACTOR_NAME);
		return;
	}

	/// Check boxCQC availability & bind
	boxCQC = owner->GetBoxCQC();
	if (!boxCQC)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get BoxCQC on " + owner->ACTOR_NAME);
		return;
	}
	defaultCollisionProfileCQC = boxCQC->GetCollisionProfileName();
	boxCQC->OnComponentBeginOverlap.AddUniqueDynamic(this, &UAttackComponent::BoxCQCBeginOverlap);

	/// Check offsetShoot availability
	offsetShoot = owner->GetOffsetShoot();
	if (!offsetShoot)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get offsetShoot on " + owner->ACTOR_NAME);
		return;
	}
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
	offsetShoot->AttachToComponent(owner->GetMesh(), AttachmentRules, SHOOT_SOCKECT);

	/// check Arena availability
	arena = owner->GetArena();
	if (!arena)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "Failed to get arena on " + owner->ACTOR_NAME);
		return;
	}

	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "INIT ATTACK", CYAN);

	isInitializationGood = !disableAttack;
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!isInitializationGood)
		return;

	const int _distanceArenaToAI = FVector::Dist(arena->GetActorLocation(), owner->GetActorLocation());
	if (_distanceArenaToAI >= arena->GetTrackingRange())
		owner->PlayDeath();
}

void UAttackComponent::ResetAttack()
{
	if (!isInitializationGood)
		return;

	boxCQC->SetCollisionProfileName(defaultCollisionProfileCQC);
	trackingSystem->SetLookAtTarget(false);
	listActorsHitByCQC.Empty();
	currentCQCData = nullptr;
	currentNumberOfShoot = 0;

	GetWorld()->GetTimerManager().ClearTimer(timerShoot);
}

void UAttackComponent::StartRangeTimer(const ERangeAttack& _range)
{
	if (!mapRange.Contains(_range))
		return;

	mapRange[_range].isInCooldown = true;
	FTimerDelegate _DelegateRange;
	_DelegateRange.BindUFunction(this, FName("EndRangeTimer"), _range);
	GetWorld()->GetTimerManager().SetTimer(mapRange[_range].timerCooldown, _DelegateRange, mapRange[_range].cooldown, false);
}

void UAttackComponent::EndRangeTimer(const ERangeAttack& _range)
{
	mapRange[_range].isInCooldown = false;
	GetWorld()->GetTimerManager().ClearTimer(mapRange[_range].timerCooldown);
}

FUniqueNetIdRepl UAttackComponent::GetPlayerIdWithHighestThreat()
{
	FUniqueNetIdRepl _playerId = GETFPC->GetCharacter()->GetPlayerState()->GetUniqueId();
	return _playerId;
}

AMainCharacter* UAttackComponent::GetCharacterByNetId(FUniqueNetIdRepl _id)
{
	TArray<APlayerState*> _listPlayerState = GetWorld()->GetGameState()->PlayerArray;
	const int _listPlayerStateCount = _listPlayerState.Num();
	for (int i = 0; i < _listPlayerStateCount; i++)
	{
		APlayerState* _playerState = _listPlayerState[i];
		if (_playerState->GetUniqueId() == _id)
			return Cast<AMainCharacter>(_playerState->GetPawn());
	}
	return nullptr;
}

#pragma region CQC
void UAttackComponent::InitCQCAttack(UAttackCQCDataAsset* _CQCData)
{
	if (!isInitializationGood)
		return;

	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "START ATTACK CQC", BLUE);

	StartRangeTimer(ERangeAttack::Short);
	currentCQCData = _CQCData;
	if (!currentCQCData)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "CQC Data is null of " + OWNER_NAME);
		return;
	}

	onAttack.Broadcast();

	/// Stop owner movement
	AIMovementComponent->LockMovement();
	/// Rotate to target
	trackingSystem->SetLookAtTarget(true);
}

void UAttackComponent::StartAttack()
{
	if (!currentCQCData)
		return;

	/// Stop rotation to allow anticipation
	trackingSystem->SetLookAtTarget(false);
	/// Launch animation
	animInstance->LaunchAnimation(currentCQCData->GetAnimationToLaunch());
}

void UAttackComponent::EnableHitBox()
{
	if (!currentCQCData)
		return;
	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "ENABLE HitBox", ORANGE);
	boxCQC->SetCollisionProfileName(currentCQCData->GetCollisionProfile().Name);
}

void UAttackComponent::DisableHitBox()
{
	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "DISABLE HitBox", ORANGE);
	boxCQC->SetCollisionProfileName(defaultCollisionProfileCQC);
}

void UAttackComponent::JumpAttack()
{
	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "JUMP CQC", YELLOW);
	AMainCharacter* _target = trackingSystem->GetTarget();
	if (!_target || !currentCQCData)
		return;

	/// Calcul direction
	const FVector& _ownerLocation = owner->GetActorLocation();
	const FVector& _targetLocation = _target->GetActorLocation();
	const FVector& _targetDirection = (_targetLocation - _ownerLocation).GetSafeNormal() * moveDistance;
	FVector _launchLocation = _ownerLocation + _targetDirection;

	/// Apply or not prediction
	FVector _targetVelocity = _target->GetVelocity();
	const float _dot = FVector::DotProduct(_targetDirection.GetSafeNormal(), _targetVelocity.GetSafeNormal());
	if (_dot >= -0.25f && _dot <= 0.45f && _dot != 0.f)
	{
		const float _randImprecision = RANDOM_FLOAT_RANGE(imprecisionFactor.X, imprecisionFactor.Y);
		FVector _prediction = (_targetVelocity.GetSafeNormal() * _targetVelocity.Length()) * _randImprecision;
		_prediction.Z = 0.f;
		_launchLocation += _prediction;
	}

	/// Add height modifier 
	const float _ZDiffOwnertoTarget = _targetLocation.Z - _ownerLocation.Z;
	if (_target->GetCharacterMovement()->IsMovingOnGround())
		_launchLocation += FVector::UpVector * _ZDiffOwnertoTarget;
	if (_ZDiffOwnertoTarget > 0)
		_launchLocation += FVector::UpVector * _target->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	/// Launch owner
	FVector _outLaunchVelocity;
	const bool _isValid = UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), _outLaunchVelocity, _ownerLocation, _launchLocation, 0.f, pushSnapPower);

	if (_isValid)
	{
		_outLaunchVelocity.X = CLAMP(_outLaunchVelocity.X, -moveDistance, moveDistance);
		_outLaunchVelocity.Y = CLAMP(_outLaunchVelocity.Y, -moveDistance, moveDistance);
		owner->LaunchCharacter(_outLaunchVelocity, true, true);
	}
	else
	{
		_outLaunchVelocity = _targetDirection + FVector::UpVector * _ZDiffOwnertoTarget;
		owner->LaunchCharacter(_outLaunchVelocity, true, true);
	}

	AIMovementComponent->LookAtTarget(_launchLocation);
}

void UAttackComponent::EndAttackCQC()
{
	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "END ATTACK CQC", BLUE);
	AIController->StopMovement();
	listActorsHitByCQC.Empty();
	currentCQCData = nullptr;
	trackingSystem->SetLookAtTarget(false);
	AIMovementComponent->UnlockMovement();
	onEndAttack.Broadcast();
}

void UAttackComponent::BoxCQCBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isInitializationGood || !currentCQCData)
		return;

	/// Check OtherActor and already hit
	if (OtherActor && !listActorsHitByCQC.Contains(OtherActor))
	{
		listActorsHitByCQC.Add(OtherActor);
		/// Apply damage if possible
		ULifeComponent* _lifeComponent = OtherActor->GetComponentByClass<ULifeComponent>();
		if (_lifeComponent)
			_lifeComponent->Damage(currentCQCData->GetDamage(), owner);
		if (currentCQCData)
			currentCQCData->CQCEffect(OtherActor, owner);
	}
}
#pragma endregion

#pragma region Projectile
void UAttackComponent::AttackDistance(TSubclassOf<AProjectile> _projectileRef)
{
	if (!isInitializationGood)
		return;

	LOG_COLOR(ATTACK_COMPONENT_USE_LOG, "ATTACK DISTANCE", BLUE);

	if (!_projectileRef)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "projectile reference is null of " + OWNER_NAME);
		return;
	}

	currentProjectileStat = _projectileRef.GetDefaultObject()->GetStats();
	if (!currentProjectileStat)
	{
		LOG_ERROR(ATTACK_COMPONENT_USE_LOG, "currentProjectileStat is null of " + OWNER_NAME);
		return;
	}
	//Launch animation
	animInstance->LaunchAnimation(currentProjectileStat->GetAnimation());
	trackingSystem->SetLookAtTarget(true);

	projectileRef = _projectileRef;

	/// Shoot projectile(s)
	numberOfProjectile = currentProjectileStat->GetNumberOfProjectile();
	const float _rateProjectile = currentProjectileStat->GetCooldown();
	const float _rateBtwProjectile = currentProjectileStat->GetDelayBetweenProjectile();
	const bool _isMultipleShoot = numberOfProjectile > 1;

	GetWorld()->GetTimerManager().SetTimer(timerShoot, this, &UAttackComponent::SpawnProjectile, _isMultipleShoot ? _rateBtwProjectile : _rateProjectile, true, _isMultipleShoot ? _rateProjectile : -1);
}

FVector UAttackComponent::PredictedLocation(const FVector& _targetLocation, const FVector& _targetVelocity, const float _actorSpeed)
{
	const FVector& _ownerLocation = GetOwner()->GetActorLocation();
	const float _distance = (_targetLocation - _ownerLocation).Length();
	const float _time = _distance / _actorSpeed;
	const FVector& _newPosition = _targetLocation + (_targetVelocity * _time);

	return _newPosition;
}

void UAttackComponent::SpawnProjectile()
{
	if (!currentProjectileStat || !projectileRef)
		return;

	currentNumberOfShoot++;

	/// Spawn and init projectile
	const FVector& _location = offsetShoot->GetComponentLocation();
	const FRotator& _rotation = GetOwner()->GetActorRotation();
	FActorSpawnParameters _param = FActorSpawnParameters();
	_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const AActor* _target = trackingSystem->GetTarget();

	FVector _targetLocation = _target->GetActorLocation();
	const float _range = currentProjectileStat->GetRangeMax();
	if (FVector::Distance(_location, _targetLocation) > _range)
		_targetLocation = _location + (_targetLocation - _location).GetSafeNormal() * _range;

	FVector _targetShoot = PredictedLocation(_targetLocation, _target->GetVelocity(), currentProjectileStat->GetSpeed());

	if (currentProjectileStat->GetUsePrecision())
	{
		if (!precisionProjectile.Contains(projectileRef))
			precisionProjectile.Add(projectileRef, FProjectilePrecision(currentProjectileStat->GetInitPrecision(), currentProjectileStat->GetMaxPrecision(), currentProjectileStat->GetAddPrecision()));
		else
			precisionProjectile[projectileRef].AddPrecision();
		_targetShoot = PrecisionLocation(_targetShoot, precisionProjectile[projectileRef].currentPrecision);
	}

	AProjectile* _newProjectile = GetWorld()->SpawnActor<AProjectile>(projectileRef, _location, _rotation, _param);
	if (!_newProjectile)
	{
		LOG_WARNING(ATTACK_COMPONENT_USE_LOG, "Failed to spawn new projectile");
		GetWorld()->GetTimerManager().ClearTimer(timerShoot);
		return;
	}
	_newProjectile->InitProjectile(_targetShoot);
	_newProjectile->InitClearArena(owner->GetArena());

	/// Stop if all projectile has been shoot
	if (currentNumberOfShoot >= numberOfProjectile)
	{
		GetWorld()->GetTimerManager().ClearTimer(timerShoot);

		/// Reset ranged attack
		currentNumberOfShoot = 0;
	}
}

FVector UAttackComponent::PrecisionLocation(const FVector& _position, const int _currentPrecision) const
{
	const double _precisionX = (_currentPrecision * _position.X) / 100;
	const float _precisionYP = (_currentPrecision * _position.Y) / 100;
	const float _precisionYN = ((200 - _currentPrecision) * _position.Y) / 100;

	const float _x = FMath::RandRange(_position.X, _precisionX);
	const float _y = FMath::RandRange(_precisionYN, _precisionYP);

	return FVector(_x, _y, _position.Z);
}

void UAttackComponent::ClearTimerProjectile()
{
	trackingSystem->SetLookAtTarget(false);
	GetWorld()->GetTimerManager().ClearTimer(timerShoot);
}
#pragma endregion