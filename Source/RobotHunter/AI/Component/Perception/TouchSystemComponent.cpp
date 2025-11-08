// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchSystemComponent.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void UTouchSystemComponent::InitPerception()
{
	Super::InitPerception();

	isInitializationGood = true;
}

void UTouchSystemComponent::PerceptionBehaviour()
{
	Super::PerceptionBehaviour();

	/// Sphere overlap around owner to catch AMainCharacter too close
	TArray<AActor*> _items = {};
	const bool _overlap = UKismetSystemLibrary::SphereOverlapActors(this, GetOwnerLocation(), rangePerception, layersDetection,
		AMainCharacter::StaticClass(), { GetOwner() }, _items);

	if (_overlap)
	{
		Detected(_items[0]);
	}
}

bool UTouchSystemComponent::DrawDebug()
{
	if (!Super::DrawDebug())
		return false;

	const FVector& _ownerForward = GetOwner()->GetActorForwardVector();
	const FVector& _ownerRight = GetOwner()->GetActorRightVector();
	DRAW_CIRCLE_TIME(GetOwnerLocation(), rangePerception, FColor::Orange, _ownerForward, _ownerRight, DELTA_TICK);
	return true;
}
