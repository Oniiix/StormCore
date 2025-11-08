#include "CustomSplineActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

ACustomSplineActor::ACustomSplineActor()
{
	spline = CreateDefaultSubobject<USplineComponent>("Spline");
	spline->SetupAttachment(RootComponent);

	usePointsToolDebug = false;
	useCustomDistance = false;

	distanceBetweenPoints = 100.0f;
	numberOfPoints = 10;
}

void ACustomSplineActor::DrawDebug()
{
	if (spline)
	{
		if (usePointsToolDebug)
		{
			FVector _currentLoc = FVector();
			const float _distanceBetweenPoints = useCustomDistance ? distanceBetweenPoints : spline->GetSplineLength() / (numberOfPoints - 1);

			for (int i = 0; i < numberOfPoints; i++)
			{
				_currentLoc = spline->GetLocationAtDistanceAlongSpline(_distanceBetweenPoints * i, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), _currentLoc, 25.0f, 10.0f, FColor::Blue);
			}
		}
	}
}

void ACustomSplineActor::SnapSplinePoint()
{
	SnapSpline(spline);
}

void ACustomSplineActor::SnapSpline(USplineComponent* _spline)
{
	const int _pointCount = GetNumberOfPoints();
	FHitResult _result;

	if (_pointCount < 2)
		return;

	TArray<FVector> _newPointsPosition;

	for (int i = 0; i < _pointCount; i++)
	{
		FVector _pointsPosition = _spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		if (UKismetSystemLibrary::LineTraceSingleForObjects(this, _pointsPosition, _pointsPosition + FVector::DownVector * 10000, snapMask, false, {}, EDrawDebugTrace::None, _result, true))
			_newPointsPosition.Add(_result.ImpactPoint + snapOffset);
		else
			_newPointsPosition.Add(_pointsPosition);
	}

	SetActorLocation(_newPointsPosition[0]);

	for (size_t i = 0; i < _pointCount; i++)
		_spline->SetLocationAtSplinePoint(i, _newPointsPosition[i], ESplineCoordinateSpace::World, true);
}

void ACustomSplineActor::RefreshPoints()
{
	if (spline)
	{
		RefreshPointSpline(spline);
	}
}

void ACustomSplineActor::RefreshPointSpline(USplineComponent* _spline)
{
	const float _splineLength = _spline->GetSplineLength();
	const float _distanceBetweenPoints = useCustomDistance ? distanceBetweenPoints : _splineLength / (numberOfPoints - 1);

	TArray<FSplinePoint> _pointsData = TArray<FSplinePoint>();
	FSplinePoint _splinePoint = FSplinePoint();
	FVector _tangent = FVector();
	float _currentDistance = 0.0f;
	bool _endLoop = false;

	for (int i = 1; i < numberOfPoints; i++)
	{
		_currentDistance = _distanceBetweenPoints * i;

		if (i == numberOfPoints - 1)
			_currentDistance = _splineLength;

		if (_currentDistance > _splineLength)
		{
			_currentDistance = _splineLength;
			_endLoop = true;
		}

		_tangent = _spline->GetTangentAtDistanceAlongSpline(_currentDistance, ESplineCoordinateSpace::World);
		_splinePoint.ArriveTangent = _tangent;
		_splinePoint.LeaveTangent = _tangent;


		_splinePoint.Position = _spline->GetLocationAtDistanceAlongSpline(_currentDistance, ESplineCoordinateSpace::Local);
		_splinePoint.Type = ESplinePointType::Curve;
		_splinePoint.InputKey = i;
		_pointsData.Add(_splinePoint);

		if (_endLoop)
			break;
	}

	const int _pointsCount = _spline->GetNumberOfSplinePoints();

	for (int i = _pointsCount - 1; i > 0; i--)
		_spline->RemoveSplinePoint(i);

	const int _pointsDataCount = _pointsData.Num();

	for (int i = 0; i < _pointsDataCount; i++)
		_spline->AddPoint(_pointsData[i]);
}
