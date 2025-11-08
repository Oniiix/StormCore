#include "DayManager.h"
#include <Net/UnrealNetwork.h>
#include "RobotHunter/Utils/DebugUtils.h"

ADayManager::ADayManager()
{
	PrimaryActorTick.bCanEverTick = true;
	dayCounter = 1;
}

void ADayManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADayManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ADayManager::UpdateDay()
{
	if (dayCounter == 3)
		dayCounter = 3;
	else
		dayCounter += 1;
}

void ADayManager::LoadActualDay(int _actualDay)
{
	dayCounter = _actualDay;
}
