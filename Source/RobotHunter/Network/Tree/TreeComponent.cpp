// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeComponent.h"
#if WITH_EDITOR
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#endif
#include "RobotHunter/Utils/DebugUtils.h"

UTreeComponent::UTreeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTreeComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!isCustomLoading)
		onLoadStateChanged.AddUniqueDynamic(this, &UTreeComponent::BasicChangeState);
}



#if WITH_EDITOR
void UTreeComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	GUnrealEd->UpdateFloatingPropertyWindows();
}
#endif

void UTreeComponent::ChangeToken(const bool _state)
{
	token = _state ? token + 1 : token - 1 < 0 ? 0 : token - 1;

	if ((_state && token == 1) || token == 0)
		onLoadStateChanged.Broadcast(_state);
}


void UTreeComponent::BasicChangeState(const bool _state)
{
	USceneComponent* _rootComp = GetOwner()->GetRootComponent();
	if (!_rootComp)
		return;

	_rootComp->SetVisibility(_state, true);
	_rootComp->GetOwner()->SetActorEnableCollision(_state);
}
