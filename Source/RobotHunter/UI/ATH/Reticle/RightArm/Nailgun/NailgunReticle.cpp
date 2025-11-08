// Objectif 3D Copyright


#include "NailgunReticle.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/NailGun/NailGunActor.h"

void UNailgunReticle::InitWithDelay()
{
	ARightArmActor* _arm = GetRightMember();
	CHECK_NULL(_arm, "Right arm is null");
	nailGunActor = Cast<ANailGunActor>(_arm);
	CHECK_NULL(nailGunActor, "nailgun is null");
	Super::InitWithDelay();

	//maxCharge = nailGunActor->GetMaxNotchQuantity();
}

void UNailgunReticle::RegisterEvents()
{
	Super::RegisterEvents();
	CHECK_NULL(nailGunActor, "Register Event : Nail gun is null");
	///BIND_ACTION(nailGunActor, OnUpdateCount, UNailgunReticle, OnUpdateCounted);
	//BIND_ACTION(nailGunActor, OnRelease, UNewNailgunCrosshair, OnUpdateCounted);
}


void UNailgunReticle::OnUpdateCounted(float _count)
{
	currentPhase = _count - 1;
	//maxCharge = nailGunActor->GetMaxNotchQuantity();
	fillPercentage = (currentPhase + 1) / maxCharge;
	Invalidate(EInvalidateWidget::PaintAndVolatility);
}


int32 UNailgunReticle::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                   const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                   int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D _center = AllottedGeometry.GetLocalSize() * 0.5f;

	const float _totalSegmentAngle = 360.0f / maxCharge;

	const float _segmentDrawAngle = _totalSegmentAngle - spaceAngle;

	int _segmentsToDraw = currentPhase + 1;

	for (int i = 0; i < _segmentsToDraw; i++)
	{
		float _startAngle = FMath::DegreesToRadians((i * _totalSegmentAngle) + startAngleCircle); // Début du segment
		float _endAngle = FMath::DegreesToRadians((i * _totalSegmentAngle + _segmentDrawAngle) + startAngleCircle);
		// Fin du segment

		DrawSegment(OutDrawElements, AllottedGeometry, _center, _startAngle, _endAngle, LayerId);
	}

	return LayerId + 1;
}

void UNailgunReticle::DrawSegment(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
                                  const FVector2D& _center, float _startAngle, float _endAngle, int32 LayerId) const
{
	//constexpr int _numpoints = 20;
	//const float _angleincrement = (_endangle - _startangle) / _numpoints;

	//fvector2d _startpoint(
	//	_center.x + radius * fmath::cos(_startangle),
	//	_center.y + radius * fmath::sin(_startangle)
	//);

	//for (int i = 0; i < _numpoints; i++)
	//{
	//	float _nextangle = _startangle + (i + 1) * _angleincrement;

	//	fvector2d _endpoint(
	//		_center.x + radius * fmath::cos(_nextangle),
	//		_center.y + radius * fmath::sin(_nextangle)
	//	);

	//	drawline(outdrawelements, allottedgeometry, _startpoint, _endpoint, flinearcolor::white, segmentthickness,
	//	         layerid);

	//	_startpoint = _endpoint;
	//}
}

void UNailgunReticle::DrawLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
                               const FVector2D& Start, const FVector2D& End, const FLinearColor& Color,
                               float LineThickness, int32 LayerId) const
{
	FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
	                             TArray<FVector2D>({Start, End}), ESlateDrawEffect::None, Color, true, LineThickness);
}
