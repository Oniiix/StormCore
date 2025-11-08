// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolNavLink.h"
#include "NavLinkCustomComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"

AToolNavLink::AToolNavLink()
{
	startSpline = CreateDefaultSubobject<UCustomSplineComponent>("StartSpline");
	startSpline->SetupAttachment(mesh);
}

void AToolNavLink::SpawnNavLink()
{
	ClearAllNavLink();
	for (int i = 0; i < startSpline->GetNumberOfSplinePoints(); i++)
	{
		const FVector& _positionPoint = startSpline->GetLocationAtSplineInputKey(i, ESplineCoordinateSpace::World);
		ANavLinkJumpAI* _navLink = GetWorld()->SpawnActor<ANavLinkJumpAI>(navLink, _positionPoint, FRotator::ZeroRotator);
		MovePointNavLink(_navLink, i);
		allNavLink.Add(_navLink);
	}
}

void AToolNavLink::ClearAllNavLink()
{
	if (allNavLink.IsEmpty())
		return;
	for (int i = 0; i < allNavLink.Num(); i++)
	{
		if (!allNavLink[i])
			continue;
		allNavLink[i]->Destroy();
	}

	allNavLink.Empty();
}

void AToolNavLink::RefreshStartSpline()
{
	startSpline->SetCanAddPoint(false);
	const int _numPointStartPoint = startSpline->GetNumberOfSplinePoints();
	for (int i = 0; i < GetNumberOfPoints(); i++)
	{
		if (i < _numPointStartPoint)
			startSpline->SetLocationAtSplinePoint(i, spline->GetLocationAtSplineInputKey(i, ESplineCoordinateSpace::Local), ESplineCoordinateSpace::Local);
		else
			startSpline->AddSplineLocalPoint(spline->GetLocationAtSplineInputKey(i, ESplineCoordinateSpace::Local));
	}
	startSpline->SetCanAddPoint(true);
}

void AToolNavLink::AddPointSpline(const int _index)
{
	if (startSpline->GetNumberPoint() == GetNumberOfPoints())
		return;

	const FVector& _endPosition = startSpline->GetLocationAtSplineInputKey(_index, ESplineCoordinateSpace::Local);
	spline->AddSplineLocalPoint(_endPosition);
}

void AToolNavLink::RemovePointSpline(const int _index)
{
	if (startSpline->GetNumberPoint() == GetNumberOfPoints())
		return;
	spline->RemoveSplinePoint(_index);
}

void AToolNavLink::MovePointNavLink(ANavLinkJumpAI* _navLink, const int _indexSpline)
{
	const FVector& _positionPoint = spline->GetLocationAtSplineInputKey(_indexSpline, ESplineCoordinateSpace::World) - _navLink->GetActorLocation();
	_navLink->GetSmartLinkComp()->SetLinkData(FVector(0, 0, 0), _positionPoint, ENavLinkDirection::BothWays);
}

void AToolNavLink::RefreshPoints()
{
	canUseTimer = false;
	RefreshPointSpline(startSpline);
	const int _nbr = startSpline->GetNumberPoint();
	for (int i = 0; i < _nbr; i++)
		spline->SetLocationAtSplinePoint(i, startSpline->GetLocationAtSplineInputKey(i, ESplineCoordinateSpace::Local), ESplineCoordinateSpace::Local);

	canUseTimer = true;
}

void AToolNavLink::SnapSplinePoint()
{
	SnapSpline(startSpline);
	Super::SnapSplinePoint();
}

void AToolNavLink::SnapSpline(USplineComponent* _spline)
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

	_spline->SetWorldLocation(_newPointsPosition[0]);

	for (size_t i = 0; i < _pointCount; i++)
		_spline->SetLocationAtSplinePoint(i, _newPointsPosition[i], ESplineCoordinateSpace::World, true);
}
