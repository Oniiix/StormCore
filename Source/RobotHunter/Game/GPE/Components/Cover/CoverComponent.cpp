#include "CoverComponent.h"

UCoverComponent::UCoverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCoverComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCoverComponent::Init(UAIRangePositionComponent* _rangeAI)
{
	rangeAI = _rangeAI;
	owner = GetOwner();
	actorLocation = owner->GetActorLocation();

	listPoint = _rangeAI->CheckCollisionAllPoint(true);
	const float _num = listPoint.Num();

	rightLeftCosAngle = FMath::Cos(FMath::DegreesToRadians(rightLeftAngle));
	forwardBackwardCosAngle = FMath::Cos(FMath::DegreesToRadians(forwardBackwardAngle));

	for (FPointRangeArena _point : listPoint)
	{
		InitDirectionPoint(_point.position, _point);
	}

}

void UCoverComponent::InitDirectionPoint(const FVector& _neighborLocation, const FPointRangeArena& _point)
{
	const FVector& _forwardVector = owner->GetActorForwardVector();
	const FVector& _rightVector = owner->GetActorRightVector();
	const FVector& _toNeighbor = (_neighborLocation - actorLocation).GetSafeNormal();

	const float _forwardDot = FVector::DotProduct(_toNeighbor, _forwardVector);
	const float _rightDot = FVector::DotProduct(_toNeighbor, _rightVector);

	if (_forwardDot >= forwardBackwardCosAngle)
	{
		listFwdPoint.Add(_point);
		return;
	}
	if (_forwardDot <= -forwardBackwardCosAngle)
	{
		listBackPoint.Add(_point);
		return;
	}

	if (_rightDot >= rightLeftCosAngle)
	{
		listRightPoint.Add(_point);
		return;
	}
	if (_rightDot <= -rightLeftCosAngle)
	{
		listLeftPoint.Add(_point);
		return;
	}

}

FPointRangeArena UCoverComponent::GetPoint(const FVector& _playerLocation, const FVector& _aiLocation)
{
	FPointRangeArena _targetPoint;
	float _dist = TNumericLimits<int>().Max();

	const FVector& _bounds = owner->GetRootComponent()->GetLocalBounds().BoxExtent;
	const FVector& _scale = owner->GetActorScale();
	const FVector& _direction = (actorLocation - _playerLocation).GetSafeNormal();

	const float _extent = (_bounds.X * _scale.X) + (_bounds.Y * _scale.Y);
	
	const FVector& _targetLocation = actorLocation + _direction * _extent;

	TArray<FPointRangeArena> _currentList;

	const FVector& _forwardVector = owner->GetActorForwardVector();
	const FVector& _rightVector = owner->GetActorRightVector();

	const float _forwardDot = FVector::DotProduct(_targetLocation, _forwardVector);
	const float _rightDot = FVector::DotProduct(_targetLocation, _rightVector);

	if (_forwardDot >= forwardBackwardCosAngle)
		_currentList = listFwdPoint;

	else if (_forwardDot <= -forwardBackwardCosAngle)
		_currentList = listBackPoint;

	else if (_rightDot >= rightLeftCosAngle)
		_currentList = listRightPoint;

	else
		_currentList = listLeftPoint;
	
	for (FPointRangeArena _point : _currentList)
	{
		if (!rangeAI->PointIsAvailable(_point))
			continue;

		const float _newDistance = FVector::Dist(_point.position, _targetLocation);
		if (_dist > _newDistance)
		{
			_targetPoint = _point;
			_dist = _newDistance;

		}
	}

	return _targetPoint;
}
