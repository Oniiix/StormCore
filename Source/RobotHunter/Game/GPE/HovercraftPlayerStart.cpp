


#include "HovercraftPlayerStart.h"
#include "Components/CapsuleComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"

AHovercraftPlayerStart::AHovercraftPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetCapsuleComponent()->SetMobility(EComponentMobility::Movable);
	GetCapsuleComponent()->SetHiddenInGame(false);

	PrimaryActorTick.bCanEverTick = true;
}


