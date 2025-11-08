#include "CustomActorComponent.h"

UCustomActorComponent::UCustomActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCustomActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomActorComponent::SetupAttachment(USceneComponent* _root)
{
	//actorComponent->SetupAttachment(_root);
}