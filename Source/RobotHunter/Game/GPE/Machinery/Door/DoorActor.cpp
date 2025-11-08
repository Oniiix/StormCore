// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	baseLeft = CreateDefaultSubobject<USceneComponent>("BaseLeft");
	baseLeft->SetupAttachment(mesh);

	baseRight = CreateDefaultSubobject<USceneComponent>("BaseRight");
	baseRight->SetupAttachment(mesh);


	leftMesh = CreateDefaultSubobject<UStaticMeshComponent>("LeftMesh");
	leftMesh->SetupAttachment(baseLeft);

	rightMesh = CreateDefaultSubobject<UStaticMeshComponent>("RightMesh");
	rightMesh->SetupAttachment(baseRight);
}

void ADoorActor::InitReceiver()
{
	Super::InitReceiver();
	openSpeed = maxAngle / openTime;
	closeSpeed = maxAngle / closeTime;

	hasAuthority = HasAuthority();
}

void ADoorActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (hasFinishAction)
		return;
	isReceiverActive ? OpenDoor() : CloseDoor();
}


void ADoorActor::Action()
{
	Super::Action();
	InteractDoor();
}

void ADoorActor::InteractDoor()
{
	FTimerManager& _timerManager = GetWorldTimerManager();
	if (_timerManager.IsTimerActive(timerStopMove))
	{
		const float _time = isReceiverActive ? openTime - _timerManager.GetTimerRemaining(timerStopMove) : closeTime - _timerManager.GetTimerRemaining(timerStopMove);
		_timerManager.ClearTimer(timerStopMove);
		_timerManager.SetTimer(timerStopMove, this, &ADoorActor::StopMove, _time);
		return;
	}
	_timerManager.SetTimer(timerStopMove, this, &ADoorActor::StopMove, isReceiverActive ? openTime : closeTime);
	hasFinishAction = false;
}

void ADoorActor::OpenDoor()
{
	FRotator _initRotate = baseRight->GetRelativeRotation();
	FRotator _newRotate = FMath::Lerp(_initRotate, _initRotate - FRotator(0, 1, 0), GetWorld()->DeltaTimeSeconds * openSpeed);
	baseRight->SetRelativeRotation(_newRotate);

	_initRotate = baseLeft->GetRelativeRotation();
	_newRotate = FMath::Lerp(_initRotate, _initRotate + FRotator(0, 1, 0), GetWorld()->DeltaTimeSeconds * openSpeed);
	baseLeft->SetRelativeRotation(_newRotate);

	//TODO trouver un moyen de changer ça / avec un ratio + faire une seul fois le calcul de l'angle =>  
	//non, c'est pas en anglais je comprend pas 
}

void ADoorActor::CloseDoor()
{
	FRotator _initRotate = baseRight->GetRelativeRotation();
	FRotator _newRotate = FMath::Lerp(_initRotate, _initRotate + FRotator(0, 1, 0), GetWorld()->DeltaTimeSeconds * closeSpeed);
	baseRight->SetRelativeRotation(_newRotate);

	_initRotate = baseLeft->GetRelativeRotation();
	_newRotate = FMath::Lerp(_initRotate, _initRotate - FRotator(0, 1, 0), GetWorld()->DeltaTimeSeconds * closeSpeed);
	baseLeft->SetRelativeRotation(_newRotate);
}

void ADoorActor::StopMove()
{
	hasFinishAction = true;
	GetWorldTimerManager().ClearTimer(timerStopMove);
	if (!isReceiverActive)
		onCloseDoor.Broadcast();
}
