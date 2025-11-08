// Objectif 3D Copyright


#include "MovableComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"

UMovableComponent::UMovableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UMovableComponent::Launch(const FVector& _direction)
{
	if (launchSettings)
	{
		onLaunch.Broadcast();
		if (OWNER_AUTHORITY)
			currentLaunchSettings->Launch(_direction / weight);
		else
			GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_LaunchMovable(this, _direction);
	}
}

void UMovableComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!launchSettings)
		return;

	currentLaunchSettings = NewObject<ULaunchSettings>(this, launchSettings);
	if (currentLaunchSettings)
		currentLaunchSettings->Init(GetOwner());
}

