#include "DOTStatus.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"

void UDOTStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	if (_actor)
	{
		ULifeComponent* _lifeComp = _actor->GetComponentByClass<ULifeComponent>();

		if (_lifeComp)
			_lifeComp->Damage(params.value, source);
	}
}
