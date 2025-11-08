#include "RailActor.h"
#include "../../Game/Interactable/Lever/LeverActor.h"
#include "../../Game/GPE/Transmitter/TargetActor/RailsTargetActor.h"
#include "../../Utils/DebugUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Game/Interactable/Wagons/WagonActor.h"

ARailActor::ARailActor()
{
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	useDebug = true;

	debugSphereRadius = 10.0f;

	arrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	arrowComponent->SetupAttachment(RootComponent);

	isLocked = false;

	departureStopIndex = 0;
	returnStopIndex = 0;

	/*leverType = nullptr;
	departureLever = nullptr;
	returnLever = nullptr;*/

	targetType = nullptr;
	departureTarget = nullptr;
	returnTarget = nullptr;

	blockedRailDetected = FBlockedRailDetected();
	onPathRailDetected = FOnPathRailDetected();
	railNotDetectedAnymore = FRailNotDetectedAnymore();

	isBlockedOnDepartureExit = false;
	isBlockedOnReturnExit = false;
}

//ALeverActor* ARailActor::SpawnLever(const bool _isDepartureLever)
//{
//	if (!leverType)
//		return nullptr;
//
//	ALeverActor* _lever = GetWorld()->SpawnActor<ALeverActor>(leverType);
//	//_lever->SetIsReverse(false);
//	_lever->AddRail(this, !_isDepartureLever);
//	_lever->SetActorLocation(GetLocationAtPointIndex(GetStopIndex(_isDepartureLever)));
//	return _lever;
//}

//ALeverActor* ARailActor::SpawnReverseLever(const bool _isDepartureLever)
//{
//	if (!leverType)
//		return nullptr;
//
//	ALeverActor* _lever = GetWorld()->SpawnActor<ALeverActor>(leverType);
//	_lever->SetIsReverse(true);
//	_lever->AddRail(this, _isDepartureLever);
//	_lever->SetActorLocation(GetLocationAtPointIndex(GetStopIndex(_isDepartureLever)));
//	return _lever;
//}

ARailsTargetActor* ARailActor::SpawnTarget(const bool _isDepartureTarget)
{
	if (!targetType)
		return nullptr;

	ARailsTargetActor* _target = GetWorld()->SpawnActor<ARailsTargetActor>(targetType);
	_target->AddRail(this, !_isDepartureTarget);
	_target->SetActorLocation(GetLocationAtPointIndex(GetStopIndex(_isDepartureTarget)));
	return _target;
}

void ARailActor::UpdateArrowLocRot()
{
	if (!arrowComponent || !spline)
		return;

	const FVector _pointLoc = GetLocationAtPointIndex(GetStopIndex(true));
	arrowComponent->SetWorldTransform(FTransform(_pointLoc));

	const FVector _targetLoc = spline->GetLocationAtSplineInputKey(departureStopIndex, ESplineCoordinateSpace::World);

	const FRotator _lookAt = UKismetMathLibrary::FindLookAtRotation(_pointLoc, _targetLoc);
	arrowComponent->SetWorldRotation(_lookAt);
}

void ARailActor::DrawDebug()
{
	Super::DrawDebug();

	FColor _debugColor = isLocked ? FColor::Red : FColor::Orange;
	FVector _sphereLoc = FVector();

	_sphereLoc = GetLocationAtPointIndex(GetStopIndex(true));
	DrawDebugSphere(GetWorld(), _sphereLoc, debugSphereRadius, 10, _debugColor);

	_debugColor = isLocked ? FColor::Purple : FColor::Magenta;
	_sphereLoc = GetLocationAtPointIndex(GetStopIndex(false));
	DrawDebugSphere(GetWorld(), _sphereLoc, debugSphereRadius, 10, _debugColor);
}

void ARailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isRuntime)
		UpdateArrowLocRot();
}

void ARailActor::SnapSplinePoint()
{
	Super::SnapSplinePoint();

#if WITH_EDITOR
	RerunConstructionScripts();
#endif
}

void ARailActor::SpawnDepartureTarget()
{
	if (departureTarget)
		return;

	departureTarget = SpawnTarget(true);

	if (departureTarget)
		departureTarget->SpawnDepartureRail();
}

void ARailActor::SpawnReturnTarget()
{
	if (returnTarget)
		return;

	returnTarget = SpawnTarget(false);

	if (returnTarget)
		returnTarget->SpawnReturnRail();
}

//void ARailActor::SpawnDepartureLever()
//{
//	if (departureLever)
//		return;
//
//	departureLever = SpawnLever(true);
//
//	if (departureLever)
//		departureLever->SpawnDepartureRail();
//}

//void ARailActor::SpawnReverseDepartureLever()
//{
//	if (departureLever)
//		return;
//
//	departureLever = SpawnReverseLever(true);
//
//	if (departureLever)
//		departureLever->SpawnReturnRail();
//}

//void ARailActor::SpawnReturnLever()
//{
//	if (returnLever)
//		return;
//
//	returnLever = SpawnLever(false);
//
//	if (returnLever)
//		returnLever->SpawnReturnRail();
//}

//void ARailActor::SpawnReverseReturnLever()
//{
//	if (returnLever)
//		return;
//
//	returnLever = SpawnReverseLever(false);
//
//	if (returnLever)
//		returnLever->SpawnDepartureRail();
//}

