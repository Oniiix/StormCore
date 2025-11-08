// Fill out your copyright notice in the Description page of Project Settings.


#include "AffectByWCKComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"

UAffectByWCKComponent::UAffectByWCKComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAffectByWCKComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!wckSettings)
		return;

	currentWCKSettings = NewObject<ULaunchSettings>(this, wckSettings);
	if (currentWCKSettings)
		currentWCKSettings->Init(GetOwner());
}

void UAffectByWCKComponent::Affect(const FVector& _dir)
{
	if (currentWCKSettings)
		currentWCKSettings->Launch(_dir / weight);
}
