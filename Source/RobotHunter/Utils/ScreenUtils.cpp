// Objectif 3D Copyright


#include "ScreenUtils.h"

#include "DebugUtils.h"
#include "Widgets/Layout/Anchors.h"


float UScreenUtils::GetMargin(const FVector2D& _reference, const FVector2D& _size, const EMarginSide _side)
{
	FVector2D _screen = _reference;
	const float _result = (_side == EMarginSide::Left ? _screen.X - _size.X : _screen.Y - _size.Y) / 2.f;
	return _result < 0 ? 0 : _result;
}

FVector2D UScreenUtils::GetScreenViewport()
{
	FVector2D _screen;
	if (GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(_screen);
	else
		_screen = FVector2D(1920, 1080);
	return _screen;
}

bool UScreenUtils::VectorContainsInTargetZone(const FVector2D& _vector, const FVector2D& _reference)
{
	const FVector2D _screen = GetScreenViewport();
	const float _marginLR = GetMargin(_screen, _reference, EMarginSide::Left);
	const float _marginTB = GetMargin(_screen, _reference, EMarginSide::Top);

	const float _x = NORMALIZE(_vector.X, 0, _screen.X);
	const float _y = NORMALIZE(_vector.Y, 0, _screen.Y);

	FAnchors _anchorsZone = GetAnchorsTargetZone(_screen, _marginLR, _marginTB);
	return _x >= _anchorsZone.Minimum.X && _x <= _anchorsZone.Maximum.X &&
		_y >= _anchorsZone.Minimum.Y && _y <= _anchorsZone.Maximum.Y;
}

FAnchors UScreenUtils::GetAnchorsTargetZone(FVector2D _screen, float _marginLR, float _marginTB)
{
	const float _minX = NORMALIZE(_marginLR, 0, _screen.X);
	const float _maxX = 1.f - _minX;
	const float _minY = NORMALIZE(_marginTB, 0, _screen.Y);
	const float _maxY = 1.f - _minY;

	return FAnchors(_minX, _minY, _maxX, _maxY);
}
