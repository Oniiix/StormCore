#include "ResistantStatus.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"

void UResistantStatus::ApplyStatus(AActor* _actor)
{
	Super::ApplyStatus(_actor);

	if (statusComponent)
		statusComponent->AddResistance(source->GetClass(), this);
}

void UResistantStatus::UnapplyStatus(AActor* _actor)
{
	if (statusComponent)
		statusComponent->RemoveResistance(source->GetClass());

	Super::UnapplyStatus(_actor);
}
