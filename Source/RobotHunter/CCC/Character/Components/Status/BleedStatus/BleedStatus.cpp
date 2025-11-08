#include "BleedStatus.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"

UBleedStatus::UBleedStatus()
{
	bleedingActor = nullptr;

	bleedRate = 1.0f;
	bleedTimer = FTimerHandle();
}

void UBleedStatus::Bleed()
{
	if (bleedingActor)
	{
		ULifeComponent* _lifeComp = bleedingActor->GetComponentByClass<ULifeComponent>();

		if (_lifeComp)
		{
			_lifeComp->Damage(params.value, source);
			//LOG_COLOR_TIME(true, bleedingActor->GetName() + " bleed [" + FString::SanitizeFloat(params.value) + "]", FColor::Red, 1.0f);
		}
	}
}

void UBleedStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	bleedingActor = _actor;

	FTimerManager& _tm = GetWorld()->GetTimerManager();

	if (!_tm.IsTimerActive(bleedTimer))
		_tm.SetTimer(bleedTimer, this, &UBleedStatus::Bleed, bleedRate, true);
}

void UBleedStatus::UnapplyStatus(AActor* _actor)
{
	GetWorld()->GetTimerManager().ClearTimer(bleedTimer);
	bleedingActor = nullptr;

	Super::UnapplyStatus(_actor);
}