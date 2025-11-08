#include "ProtectedStatus.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"

void UProtectedStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	if (_actor)
	{
		ULifeComponent* _lifeComp = _actor->GetComponentByClass<ULifeComponent>();

		if (_lifeComp)
		{
			_lifeComp->SetProtection(params.value);
			_lifeComp->SetProtectDirectionalAngle(params.shieldDirectionalAngle);
			_lifeComp->SetProtectAngle(params.shieldAngle);
		}
	}
}

void UProtectedStatus::UnapplyStatus(AActor* _actor)
{
	if (_actor)
	{
		ULifeComponent* _lifeComp = _actor->GetComponentByClass<ULifeComponent>();

		if (_lifeComp)
		{
			_lifeComp->SetProtection(0.0f);
			_lifeComp->SetProtectDirectionalAngle(0.0f);
			_lifeComp->SetProtectAngle(360.0f);
		}
	}

	Super::UnapplyStatus(_actor);
}
