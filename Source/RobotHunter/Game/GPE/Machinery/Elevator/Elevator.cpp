
#include "Elevator.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/GPE/Machinery/ReceiverSystem/TimerReceiverSystem.h"

AElevator::AElevator()
{
	spline = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	spline->SetupAttachment(mesh);
}

void AElevator::InitReceiver()
{
	Super::InitReceiver();
	if (!receiverSystem)
	{
		LOG_ERROR(true, "AElevator : receiverSystem null");
		return;
	}
	InitElevator();
	BindEvent();
}

void AElevator::InitElevator()
{
	currentIndex = initIndex;
	maxNbrPoint = spline->GetNumberOfSplinePoints();
	for (int i = 0; i < maxNbrPoint; i++)
		positionAllPoint.Add(spline->GetWorldLocationAtSplinePoint(i));

	currentTargetPoint = positionAllPoint[currentIndex];

	SetActorLocation(currentTargetPoint);
}

void AElevator::BindEvent()
{
	receiverSystem->SetCustomActivation(true);
	switch (type)
	{
	case EReceiverType::Switch:
		receiverSystem->OnActivate().AddUniqueDynamic(this, &AElevator::ActionSwitch);
		onStopMove.AddUObject(this, &AElevator::StopMoveSwitch);
		break;
	case EReceiverType::Timer:
		timeTypeTimer = Cast<UTimerReceiverSystem>(receiverSystem)->GetActivationTime();
		initPointLocation = currentTargetPoint;
		receiverSystem->OnActivate().AddUniqueDynamic(this, &AElevator::ActionTimer);
		onStopMove.AddUObject(this, &AElevator::StopMoveTimer);
		break;
	case EReceiverType::Endless:
		receiverSystem->OnActivate().AddUniqueDynamic(this, &AElevator::ActionSwitch);
		onStopMove.AddUObject(this, &AElevator::StopMoveTimer);
		break;
	default:
		return;
	}
}


void AElevator::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);

	if (hasFinishAction)
		return;

	if (FVector::Distance(mesh->GetComponentLocation(), currentTargetPoint) <= 10)
		onStopMove.Broadcast();



	Move();
}

void AElevator::Action()
{
	Super::Action();
	canStopElevator = true;
	GetWorldTimerManager().SetTimer(timerStop, this, &AElevator::ChangePoint, timeStop);
}


void AElevator::Move()
{
	const FVector& _currentPosition = FMath::VInterpConstantTo(mesh->GetComponentLocation(), currentTargetPoint, GetWorld()->DeltaTimeSeconds , speed);
	SetActorLocation(_currentPosition);
}

void AElevator::ChangePoint()
{
	GetWorldTimerManager().ClearTimer(timerStop);
	if (currentIndex == maxNbrPoint - 1 || (currentIndex == 0 && returnMode))
		returnMode = !returnMode;

	returnMode ? currentIndex-- : currentIndex++;
	hasFinishAction = false;
	currentTargetPoint = positionAllPoint[currentIndex];
}

void AElevator::ActionSwitch()
{
	if (canStopElevator)
		return;
	Action();
}

void AElevator::ActionTimer()
{
	if (canStopElevator)
		return;
	FTimerDelegate _timerDelegate;
	_timerDelegate.BindLambda([&]() {
		GetWorldTimerManager().ClearTimer(timerStop);

		onStopMove.RemoveAll(this);
		onStopMove.AddLambda([this]() {
			onStopMove.RemoveAll(this);
			onStopMove.AddUObject(this, &AElevator::StopMoveTimer);
			hasFinishAction = true;
			canStopElevator = false;
			});

		hasFinishAction = false;
		currentIndex = initIndex;
		currentTargetPoint = positionAllPoint[currentIndex];

		});

	GetWorldTimerManager().SetTimer(timerTypeTimer, _timerDelegate, timeTypeTimer + timeStop, FTimerManagerTimerParameters());
	Action();
}

void AElevator::StopMoveSwitch()
{
	hasFinishAction = true;
	canStopElevator = false;
}

void AElevator::StopMoveTimer()
{
	hasFinishAction = true;
	canStopElevator = false;
	ActionSwitch();
}

#if WITH_EDITOR
void AElevator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AElevator, initIndex))
		initIndex = CLAMP(initIndex, 0, spline->GetNumberOfSplinePoints() - 1);
}
#endif