// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/CustomGameMode.h"
#include "RobotHunter/UI/ATH/Game/NewGameATH.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"

ULifeComponent::ULifeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	bleedRate = 0.0f;
	bleedBonusParams = FStatusParams(EStatusType::BLEED, 0.0f, 0.0f);
}

void ULifeComponent::SpawnHitValue(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	CHECK_NULL(_owner, "Owner is null");
	if (INSTANCEOF(_owner, AMainCharacter))
	{
		return;
	}
	CHECK_NULL(hitActorReference, "Actor hitValue reference  is null");
	AsyncTask(ENamedThreads::GameThread, [this, _owner, _damage]
		{
			AHitValueActor* HitActor = GetWorld()->SpawnActor<AHitValueActor>(hitActorReference,
				FTransform(
					_owner->GetActorLocation() + FVector(
						1, 0, 1)));
			HitActor->SetHitDamage(_damage, false);
		});
	ULifebarWidgetComponent* lifebar = GetOwner()->GetComponentByClass<ULifebarWidgetComponent>();
	CHECK_NULL(lifebar, "Life bar widget component is null");
	lifebar->CallbackOnHit();

	ACustomGameMode* _gm = Cast<ACustomGameMode>(GetWorld()->GetAuthGameMode());
	CHECK_NULL(_gm, "Gamemode is null");

	AGameHUD* _hud = Cast<AGameHUD>(GETFPC->GetHUD());
	CHECK_NULL(_hud, "Hud is null");
	CHECK_NULL(_hud->GetCurrentUI<UNewGameAth>(), "current UI is null");
	if (UNewGameAth* _ath = _hud->GetCurrentUI<UNewGameAth>())
	{
		_ath->ShowHitMarker();
	}
}

void ULifeComponent::InitLife()
{
	currentLife = startLife;
	lastChangeSudden = 0;
	isInit = true;

	onLifeChangeByDamage.AddUniqueDynamic(this, &ULifeComponent::SpawnHitValue);
	if (GetOwner()->IsA<AMainCharacter>())
		LOG_COLOR(true, "InitLife", ORANGE);
	OnRep_UpdateLife();
}

void ULifeComponent::Damage(const int _damage, AActor* _damageSource)
{
	if (shieldMode || !canTakeDamage)
		return;

	if (_damageSource)
		LOG_COLOR_TIME(LIFE_COMPONENT_USE_LOG, GetOwner()->GetName() + " has take " + FLOATSTR(_damage) + " from " + _damageSource->GetName(), ORANGE, 20);

	const bool _canBeProtected = CheckIfCanBeProtected(_damageSource);
	float _protection = protection + protectionBonus;
	_protection = _protection > 100.0f ? 100.0f : _protection;

	const float _reducedDamage = _canBeProtected && (_protection > 0.0f)
		? _damage - (_damage * (_protection / 100.0f))
		: _damage;

	if (OWNER_AUTHORITY)
	{
		lastChangeSudden = -_reducedDamage;
		lastDamageSource = _damageSource;

		currentLife = FMath::Clamp(currentLife - _reducedDamage, 0, maxLife);
		OnRep_UpdateLife();
	}
	else
	{
		ACustomPlayerController* _pc = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
		CHECK_NULL(_pc, "Failed to get ACustomPlayerController in " + OWNER_NAME);
		_pc->NetworkAI()->ServerRpc_SetLife(this, currentLife - _reducedDamage, -_damage, _damageSource);
	}

	Counter(_reducedDamage * (counterBonus / 100.0f), _damageSource);

	if (GetOwner()->GetClass()->IsChildOf<AMainCharacter>())
	{
		if (currentLife <= 0.0f)
		{
			if (GetCanRevive())
				Revive();
		}
	}
}

void ULifeComponent::RestoreLife(const int _heal)
{
	const float _boostedHeal = _heal * healBonus;

	if (OWNER_AUTHORITY)
	{
		lastChangeSudden = currentLife == maxLife ? 0 : _boostedHeal;
		currentLife = FMath::Clamp(currentLife + _boostedHeal, 0, maxLife);
		OnRep_UpdateLife();
	}
	else
	{
		ACustomPlayerController* _pc = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
		CHECK_NULL(_pc, "Failed to get ACustomPlayerController in " + OWNER_NAME);
		_pc->NetworkAI()->ServerRpc_SetLife(this, currentLife + _boostedHeal, _boostedHeal, lastDamageSource);
	}
}

void ULifeComponent::SetLife(const int _currentLife, const int _change, AActor* _damageSource)
{
	if (GetOwner()->IsA<AMainCharacter>())
		LOG_COLOR(true, "Set Life", BLUE);

	currentLife = _currentLife;
	lastChangeSudden = _change;
	lastDamageSource = _damageSource;

	if(isInit)
		OnRep_UpdateLife();
}

void ULifeComponent::OnRep_UpdateLife()
{
	onUpdateLife.Broadcast(GetOwner(), currentLife, lastChangeSudden, lastDamageSource);

	if (lastChangeSudden < 0)
	{
		onLifeChangeByDamage.Broadcast(GetOwner(), currentLife, lastChangeSudden, lastDamageSource);
	}
	else if (lastChangeSudden > 0)
	{
		if (GetOwner()->IsA<AMainCharacter>())
			LOG_COLOR(true, "Healed event Call", GREEN);
		onLifeChangeByHeal.Broadcast(GetOwner(), currentLife, lastChangeSudden, lastDamageSource);
	}
	if (currentLife <= 0)
	{
		if (!GetCanRevive())
		{
			AMainCharacter* _mc = Cast<AMainCharacter>(lastDamageSource);

			if (_mc && GetOwner()->IsA<AAI_Base>())
			{
				ULifeComponent* _mcLifeComp = _mc->GetLifeComponent();

				if (_mcLifeComp)
					_mcLifeComp->LifeSteal();
			}

			onDie.Broadcast();
		}
	}
}


bool ULifeComponent::CheckIfCanBeProtected(AActor* _damageSource)
{
	if (_damageSource)
	{
		if (protectAngle == 360.0f)
			return true;

		const FVector& _damageSourceLoc = _damageSource->GetActorLocation();
		const FVector& _ownerLoc = GetOwner()->GetActorLocation();
		const FRotator& _ownerRot = GetOwner()->GetActorRotation();

		const float _angleHalf = protectAngle / 2.0f;
		const float _baseYaw = protectDirectionalAngle + _ownerRot.Yaw;

		const float _minYaw = _baseYaw - _angleHalf;
		const float _maxYaw = _baseYaw + _angleHalf;

		const FRotator& _lookAt = UKismetMathLibrary::FindLookAtRotation(_ownerLoc, _damageSourceLoc);

		if (_lookAt.Yaw >= _minYaw && _lookAt.Yaw <= _maxYaw)
			return true;
	}

	return false;
}


void ULifeComponent::LifeSteal()
{
	if (lifeStealBonus > 0.0f)
		RestoreLife(lifeStealBonus);
}


void ULifeComponent::RegenLife()
{
	if (currentLife > 0.0f && currentLife < maxLife)
		RestoreLife(regenLife);
}

void ULifeComponent::Revive()
{
	const int _reviveLife = ((int)((float)maxLife) * (reviveLifePercent / 100.0f));
	RestoreLife(_reviveLife);
	canRevive = false;
}

void ULifeComponent::Counter(const int _damage, AActor* _damageSource)
{
	if (_damageSource && _damage > 0.0f)
	{
		ULifeComponent* _lifeComp = _damageSource->GetComponentByClass<ULifeComponent>();

		if (_lifeComp)
		{ 
			_lifeComp->Damage(_damage, GetOwner());
			AddBleed(_damageSource);
		}
	}
}

void ULifeComponent::AddBleed(AActor* _actor)
{
	if (_actor)
	{
		if (bleedBonusParams.value > 0.0f && bleedBonusParams.duration > 0.0f && bleedRate > 0.0f)
		{
			UStatusComponent* _statusComp = _actor->GetComponentByClass<UStatusComponent>();

			if (_statusComp)
			{
				UBleedStatus* _bleed = Cast<UBleedStatus>(_statusComp->AddStatus(GetOwner(), bleedBonusParams, true));

				if (_bleed)
					_bleed->SetBleedRate(bleedRate);
			}
		}
	}
}

void ULifeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULifeComponent, currentLife);
	DOREPLIFETIME(ULifeComponent, maxLife);
	DOREPLIFETIME(ULifeComponent, lastChangeSudden);
}
