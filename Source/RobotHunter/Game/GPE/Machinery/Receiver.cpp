// Fill out your copyright notice in the Description page of Project Settings.


#include "Receiver.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "ReceiverSystem/SwitchReceiverSystem.h"
#include "ReceiverSystem/TimerReceiverSystem.h"
#include "ReceiverSystem/EndlessReceiverSystem.h"
#include "Net/UnrealNetwork.h"


AReceiver::AReceiver()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
	shouldTickIfViewportsOnly = true;
	useDebug = true;
#endif

#if !WITH_EDITOR
	PrimaryActorTick.bCanEverTick = false;
#endif

	bReplicates = true;
}

void AReceiver::Action()
{
	isReceiverActive = !isReceiverActive;
	onAction.Broadcast(isReceiverActive);
}

void AReceiver::BeginPlay()
{
	Super::BeginPlay();
	InitReceiver();
	if (startActivate && HasAuthority())
		Action();
}

void AReceiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (useDebug)
		DrawDebug();
}

void AReceiver::DrawDebug()
{
	const int _transmittersCount = transmitters.Num();
	for (int i = 0; i < _transmittersCount; i++)
	{
		if (!transmitters[i]) continue;
		DrawDebugLine(GetWorld(), GetActorLocation(), transmitters[i]->GetActorLocation(), FColor::Yellow, false, -1, 0, 3);
	}
}



#if WITH_EDITOR
void AReceiver::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AReceiver, type))
	{
		if (receiverSystem)
			receiverSystem->DestroyComponent();

		switch (type)
		{
		case EReceiverType::Switch:
			receiverSystem = NewObject<UReceiverSystemComponent>(this, USwitchReceiverSystem::StaticClass(), FName("Swicth System"));
			break;

		case EReceiverType::Timer:
			receiverSystem = NewObject<UReceiverSystemComponent>(this, UTimerReceiverSystem::StaticClass(), FName("Timer System"));
			break;
		case EReceiverType::Endless:
			receiverSystem = NewObject<UReceiverSystemComponent>(this, UEndlessReceiverSystem::StaticClass(), FName("Endless System"));
			break;
		default:
			return;
		}

		AddInstanceComponent(receiverSystem);
		AddOwnedComponent(receiverSystem);
		receiverSystem->RegisterComponent();

		Modify();
	}
}
#endif


void AReceiver::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AReceiver, isReceiverActive);
}

void AReceiver::OnRep_UpdateIsActive()
{
	const bool _isActive = isReceiverActive;
	Action();
	isReceiverActive = _isActive;
}

