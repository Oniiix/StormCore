// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdFlower.h"
#include<RobotHunter/Game/GPE/Weathers/Frost/FrostWeather.h>
#include <RobotHunter/CCC/Character/MainCharacter.h>
#include "RobotHunter/Utils/DebugUtils.h"

#define COLD_FLOWER_USE_LOG true

AColdFlower::AColdFlower()
{
	areaEffect = CreateDefaultSubobject<USphereComponent>("BoxDetect");
	areaEffect->SetupAttachment(RootComponent);
}

void AColdFlower::Init()
{
	Super::Init();
	areaEffect->bHiddenInGame = !useDebug;

	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}
	currentFrostDuration = frostDuration;



	if (useDebug)
		LOG_WARNING(COLD_FLOWER_USE_LOG, "AColdFlower::Init");

	if (!desactivateModifier)
		if (IsItThisWeather(EMeteoStatus::FROST))
			ApplyModifierFrostDuration();
		else
		{
			RemoveModifierFrostDuration();
		}
}

void AColdFlower::ZoneEffect(TArray<AActor*> _actorWithEffect)
{
	Super::ZoneEffect(_actorWithEffect);
	for (size_t i = 0; i < _actorWithEffect.Num(); i++)
	{
		AMainCharacter* _character = Cast<AMainCharacter>(_actorWithEffect[i]);
		if (!_character)
			return;

		if (_character->GetSpeedModifier() != (float) ESpeedModifiers::COLD)
		{
			if (useDebug)
			LOG_COLOR(COLD_FLOWER_USE_LOG, "AColdFlower::ZoneEffect", FLinearColor::Blue);
			//TODO modify speed _character
			_character->ServerRpc_AddModifier(ESpeedModifiers::COLD);
		}
			
	}
}

void AColdFlower::ExitZoneEffect(AActor* _actorExitZoneEffect)
{
	Super::ExitZoneEffect(_actorExitZoneEffect);
	LOG_WARNING(COLD_FLOWER_USE_LOG, "AColdFlower: End Overlap Cold Flower ExitZoneEffect !");
	AMainCharacter* _character = Cast<AMainCharacter>(_actorExitZoneEffect);
	if (!_character)
		return;

	
	FTimerDelegate _timerDelegate;
	_timerDelegate.BindUFunction(this, FName("RemoveFrostDuration"), _character);

	//TIMER_DELEGATE(test,this, "RemoveFrostDuration", _character)
	WORLD->GetTimerManager().SetTimer(timerFrostDuration, _timerDelegate, frostDuration, false);

	
}

void AColdFlower::ApplyModifierFrostDuration()
{
	if (useDebug)
		LOG_COLOR(COLD_FLOWER_USE_LOG, "AColdFlower:ApplyModifierFrostDuration !", FLinearColor::Blue);
	frostDuration *= modifierFrostDuration;
}
void AColdFlower::RemoveModifierFrostDuration()
{
	frostDuration =currentFrostDuration;
}
void AColdFlower::RemoveFrostDuration(AMainCharacter* _character)
{
	if (useDebug)
	LOG_WARNING(COLD_FLOWER_USE_LOG, "AColdFlower:RemoveFrostDuration !");
	_character->ServerRpc_RemoveModifier(ESpeedModifiers::NOMODIFIERS);
}


