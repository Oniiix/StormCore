// Fill out your copyright notice in the Description page of Project Settings.


#include "SASActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/GPE/Machinery/ReceiverSystem/TimerReceiverSystem.h"

ASASActor::ASASActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASASActor::InitReceiver()
{
	Super::InitReceiver();
	if (allDoors.IsEmpty())
		return;
	allDoors[currentOpenedDoorIndex]->Action();
	numDoor = allDoors.Num();

	BindEvent();
}

void ASASActor::BindEvent()
{
	receiverSystem->SetCustomActivation(true);
	switch (type)
	{
	case EReceiverType::Switch:
		for (size_t i = 0; i < allDoors.Num(); i++)
			allDoors[i]->OnCloseDoor().AddUniqueDynamic(this, &ASASActor::ActivateDoor);
		receiverSystem->OnActivate().AddUniqueDynamic(this, &ASASActor::ActionSwitch);
		break;
	case EReceiverType::Timer:
		receiverSystem->OnActivate().AddUniqueDynamic(this, &ASASActor::ActionTimer);
		break;
	case EReceiverType::Endless:
		receiverSystem->OnActivate().AddUniqueDynamic(this, &ASASActor::ActionEndless);
		break;
	default:
		break;
	}
}

//void ASASActor::Action()
//{
//	if (allDoors.IsEmpty())
//		return;
//	Super::Action();
//
//	allDoors[currentOpenedDoorIndex]->Action();
//	currentOpenedDoorIndex = currentOpenedDoorIndex + 1 == allDoors.Num() ? 0 : currentOpenedDoorIndex + 1;
//}

void ASASActor::ActivateDoor()
{
	allDoors[currentOpenedDoorIndex]->Action();
}

void ASASActor::ActionSwitch()
{
	if (GetWorldTimerManager().IsTimerActive(allDoors[currentOpenedDoorIndex]->GetTimerStopMove()))
		return;

	allDoors[currentOpenedDoorIndex]->Action();
	currentOpenedDoorIndex = currentOpenedDoorIndex + 1 == allDoors.Num() ? 0 : currentOpenedDoorIndex + 1;
}

void ASASActor::ActionTimer()
{
	OpenAllDoor();
	GetWorldTimerManager().SetTimer(timerSAS, this, &ASASActor::CloseDoor, Cast<UTimerReceiverSystem>(receiverSystem)->GetActivationTime());
}


void ASASActor::ActionEndless()
{
	OpenAllDoor();
}

void ASASActor::OpenAllDoor()
{
	for (int i = 0; i < numDoor; i++)
		if (!allDoors[i]->GetIsOpen())
			allDoors[i]->Action();
}

void ASASActor::CloseDoor()
{
	const int _num = allDoors.Num();
	for (int i = 0; i < _num; i++)
		allDoors[i]->Action();
}

#if WITH_EDITOR
bool ASASActor::CanEditChange(const FProperty* InProperty) const
{
	const bool _parentVal = Super::CanEditChange(InProperty);
	if (InProperty->GetName() == GET_MEMBER_NAME_CHECKED(ASASActor, currentOpenedDoorIndex))
		return _parentVal && allDoors.Num() > 0;
	return _parentVal;
}

void ASASActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove || PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayClear)
		currentOpenedDoorIndex = FMath::Clamp(currentOpenedDoorIndex, 0, allDoors.Num() == 0 ? 0 : allDoors.Num() - 1);
}
#endif