#include "WindFlowerV2.h"
#include "Components/CapsuleComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Weathers/Wind/WindWeather.h"

AWindFlowerV2::AWindFlowerV2()
{
	effectArea = CreateDefaultSubobject<UCapsuleComponent>("EffectArea");
	effectArea->SetupAttachment(RootComponent);

	forwardForce = 1000.0f;
	upwardForce = 1000.0f;

	forwardForceModifier = 3.0f;
	upwardForceModifier = 3.0f;

	useModifier = false;
}

void AWindFlowerV2::StartActivePhase()
{
	TestWeather<AWindWeather>();
	Super::StartActivePhase();
}

void AWindFlowerV2::ApplyEffectToActorsInArea()
{
	if (actorsInArea.IsEmpty())
		return;

	const int _num = actorsInArea.Num();
	AMainCharacter* _mc = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_mc = Cast<AMainCharacter>(actorsInArea[i]);

		if (_mc)
		{
			const float _forwardForce = useModifier ? forwardForce * forwardForceModifier : forwardForce;
			const float _upwardForce = useModifier ? upwardForce * upwardForceModifier : upwardForce;

			_mc->LaunchCharacter(GetActorForwardVector() * _forwardForce + GetActorUpVector() * _upwardForce, true, true);
		}
	}
}

void AWindFlowerV2::EnableModifiers()
{
	useModifier = true;
}

void AWindFlowerV2::DisableModifiers()
{
	useModifier = false;
}
