// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplineComponent.h"
#include "RobotHunter/Splines/ToolNavLink/ToolNavLink.h"

UCustomSplineComponent::UCustomSplineComponent()
{
	numberPoint = GetNumberOfSplinePoints();
}

void UCustomSplineComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCustomSplineComponent::UpdateSpline()
{
	Super::UpdateSpline();
	if (!canAddPoint)
		return;

	if(!tool)
		tool = Cast<AToolNavLink>(GetOwner());

	if (GetNumberOfSplinePoints() > numberPoint)
	{
		numberPoint = GetNumberOfSplinePoints();
		FTimerHandle _timer; 
		tool->GetCanUseTimer() ? tool->GetWorldTimerManager().SetTimer(_timer, this, &UCustomSplineComponent::AddPoint, tool->GetTimeAddPoint()) : AddPoint();
	}

	if (GetNumberOfSplinePoints() < numberPoint)
	{
		numberPoint = GetNumberOfSplinePoints();
		tool->RemovePointSpline(numberPoint);
	}
}

void UCustomSplineComponent::AddPoint()
{
	if (tool)
		tool->AddPointSpline(numberPoint);
}
