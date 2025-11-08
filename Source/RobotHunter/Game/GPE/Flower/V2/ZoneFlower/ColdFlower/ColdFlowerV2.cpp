#include "ColdFlowerV2.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/Game/GPE/Weathers/Frost/FrostWeather.h"
#include "RobotHunter/CCC/Character/Components/Status/FreezingStatus/FreezingStatus.h"

AColdFlowerV2::AColdFlowerV2()
{
	effectArea = CreateDefaultSubobject<USphereComponent>("EffectArea");
	effectArea->SetupAttachment(RootComponent);

	freezeStackDurationMultiplier = 1.5f;
	useMuliplierWhenAddingStack = false;

	freezingStatus = FStatusParams(EStatusType::FREEZE, 1.1f, 100, 1.0f, EStatusType::FREEZE);

	stunnedStatus = FStatusParams(EStatusType::STUN, 0.1f, 10.0f);
	protectedStatus = FStatusParams(EStatusType::PROTECT, 25.0f, 10.0f, 0.0f, 360.0f);
	resistantStatus = FStatusParams(EStatusType::RES_MODIF, 100.0f, 10.0f, EStatusType::FREEZE);

	useModifiers = false;
}

void AColdFlowerV2::StartActivePhase()
{
	TestWeather<AFrostWeather>();
	Super::StartActivePhase();
}

void AColdFlowerV2::StartCooldownPhase()
{
	Super::StartCooldownPhase();

	if (actorsInArea.IsEmpty())
		return;

	const int _num = actorsInArea.Num();
	UStatusComponent* _statusComp = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_statusComp = actorsInArea[i]->GetComponentByClass<UStatusComponent>();

		if (_statusComp)
			_statusComp->RefreshStatus(this, freezingStatus, true);
	}
}

void AColdFlowerV2::ApplyEffectToActorsInArea()
{
	if (actorsInArea.IsEmpty())
		return;

	const int _num = actorsInArea.Num();
	UStatusComponent* _statusComp = nullptr;
	UFreezingStatus* _status = nullptr;

	for (size_t i = 0; i < _num; i++)
	{
		_statusComp = actorsInArea[i]->GetComponentByClass<UStatusComponent>();

		if (_statusComp)
		{
			_status = Cast<UFreezingStatus>(_statusComp->AddStatus(this, freezingStatus, true));

			if (_status)
			{
				if (useModifiers)
					_status->SetStackDurationMultiplier(useMuliplierWhenAddingStack, freezeStackDurationMultiplier);

				_status->SetMaxStackStatus(stunnedStatus, protectedStatus, resistantStatus);
			}
		}
	}
}

void AColdFlowerV2::EnableModifiers()
{
	useModifiers = true;
}

void AColdFlowerV2::DisableModifiers()
{
	useModifiers = false;
}
