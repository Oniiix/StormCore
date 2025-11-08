// Fill out your copyright notice in the Description page of Project Settings.


#include "WindFlower.h"
#include "../../../CCC/Character/MainCharacter.h"
#include <Components/CapsuleComponent.h>

#include "RobotHunter/Game/CustomGameMode.h"
#include "RobotHunter/Game/GPE/Weathers/Wind/WindWeather.h"
#include "RobotHunter/Utils/DebugUtils.h"


AWindFlower::AWindFlower()
{
	areaEffect = CreateDefaultSubobject<UCapsuleComponent>("BoxDetect");
	areaEffect->SetupAttachment(RootComponent);
}


void AWindFlower::Init()
{
	Super::Init();

	areaEffect->bHiddenInGame = !useDebug;

	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}

	if (!desactivateModifier)
		if (IsItThisWeather(EMeteoStatus::HURRICANE))
			ActiveModifier();
		else
		{
			DesactivateModifier();
		}

}
void AWindFlower::ActiveModifier()
{
	activateModifier = true;
}
void AWindFlower::DesactivateModifier()
{
	activateModifier = false;
}


void AWindFlower::ZoneEffect(TArray<AActor*> _actorWithEffect)
{
	Super::ZoneEffect(_actorWithEffect);
	for (size_t i = 0; i < _actorWithEffect.Num(); i++)
	{
		AMainCharacter* _character = Cast<AMainCharacter>(_actorWithEffect[i]);
		if (!_character || !_character->GetMesh())
			return;

		const float _windForceFWD = activateModifier == true ? windForceForward * modifierWindForceForward : windForceForward,
					_windForceUP = activateModifier == true ? windForceUp * modifierWindForceUp : windForceUp;

		_character->LaunchCharacter(GetActorForwardVector() * _windForceFWD + GetActorUpVector() * _windForceUP, true, true);
	}
}