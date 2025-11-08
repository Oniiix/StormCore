// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotHunter/AI/AI_Hunter/AI_Hunter.h"
#include "Components/CapsuleComponent.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/AI/Settings/TrapperSettingsDataAsset.h"

AAI_Hunter::AAI_Hunter() : AAI_Base(EAIType::Trapper)
{
	camouflageComponent = CreateDefaultSubobject<UCamouflageComponent>("Camouflage Component");
	AddOwnedComponent(camouflageComponent);
}

void AAI_Hunter::BeginPlay()
{
	Super::BeginPlay();

	currentDamageDodge = damageDodge;
	currentTimeDamageDodge = damageTimeDodge;
	camouflageComponent->SetSpeedCamouflage(speedCamouflage);
}

void AAI_Hunter::SetAI(AArena* _arena)
{
	UTrapperSettingsDataAsset* _trapperSettings = Cast<UTrapperSettingsDataAsset>(settings);
	if (!_trapperSettings)
	{
		LOG_ERROR(AI_TRAPPER_USE_LOG, "Failed to cast settings in Trapper' settings");
		return;
	}

	Super::SetAI(_arena);

	maxLife = lifeComponent->GetMaxLife();
	lifeComponent->OnLifeChangeByDamage().AddUniqueDynamic(this, &AAI_Hunter::DamageHunter);
	currentPercentageFlank = percentageFlank;
}

void AAI_Hunter::DamageHunter(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	const float _percentage = (_currentLife / maxLife) * 100;
	if (_percentage <= percentageLifeCritique && !isCritiqueMode)
		CritiqueMode();
	currentDamage += (_damage * -1);
	if (canDodge && !isCooldownDodge)
	{
		canDodge = false;
		FTimerHandle _timerDamage;
		GetWorldTimerManager().SetTimer(_timerDamage, this, &AAI_Hunter::StopDodge, currentTimeDamageDodge);
	}
}

void AAI_Hunter::StopDodge()
{
	if (currentDamage >= currentDamageDodge)
	{
		currentDamage = 0;
		stateTreeComponent->DodgeEnable();
		isCooldownDodge = true;
		FTimerDelegate _timer;
		_timer.BindLambda([&]() { isCooldownDodge = false; });
		GetWorldTimerManager().SetTimer(dodgeCooldownTimer, _timer, cooldownDodge, FTimerManagerTimerParameters());
	}
	canDodge = true;
}

void AAI_Hunter::CritiqueMode()
{
	isCritiqueMode = true;
	currentDamageDodge = damageDodgeCritique;
	currentTimeDamageDodge = damageTimeDodgeCritique;

	movementComponent->SetRunSpeed(speedRunCritique);
	movementComponent->SetWalkSpeed(speedWalkCritique);
	movementComponent->SetAcceleration(accelerationCritique);
	camouflageComponent->SetSpeedCamouflage(speedCamouflageCritique);

	currentPercentageFlank = percentageFlankCritique;
	movementComponent->UpdateSpeed(movementComponent->GetIsRun());
	movementComponent->UpdateAcceleration();

	if (camouflageComponent)
	{
		camouflageComponent->UpdateCamouflageStatusValue(EStatusType::PROTECT, critiqueProtectionValue);
		camouflageComponent->UpdateCamouflageStatusValue(EStatusType::STUN, critiqueStunResistanceValue);
		camouflageComponent->UpdateCamouflageStatusValue(EStatusType::STAGGER, critiqueStaggerResistanceValue);
	}
}

void AAI_Hunter::StartPlayCamouflageAnimation()
{
	startCamouflage = true;
	onStartCamouflage.Broadcast();
}

void AAI_Hunter::StartRemoveCamouflageAnimation()
{
	startDisableCamouflage = true;
	On_StartDisableCamouflage();
}

void AAI_Hunter::On_StartCamouflage()
{
	if (startCamouflage)
	{
		onStartCamouflage.Broadcast();
	}
	else
	{
		onStopCamouflage.Broadcast();
		if (!camouflageComponent)
		{
			LOG_ERROR(0, "Failed to get UCamouflageComponent");
			return;
		}

		//camouflageComponent->ServerRPC_TransformMode();
		SetCamouflageMode(true);
	}
}

void AAI_Hunter::On_StartDisableCamouflage()
{
	if (startDisableCamouflage)
	{
		onStartDisableCamouflage.Broadcast();
		if (!camouflageComponent)
		{
			LOG_ERROR(0, "Failed to get UCamouflageComponent");
			return;
		}
		camouflageComponent->DisableMesh();
	}
	else
	{
		onStopDisableCamouflage.Broadcast();
	}
}

void AAI_Hunter::StopPlayCamouflageAnimation()
{
	startCamouflage = false;
	On_StartCamouflage();


}

void AAI_Hunter::StopPlayDisableCamouflageAnimation()
{
	startDisableCamouflage = false;
	On_StartDisableCamouflage();
}

void AAI_Hunter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAI_Hunter, startCamouflage);
	DOREPLIFETIME(AAI_Hunter, startDisableCamouflage);
}
