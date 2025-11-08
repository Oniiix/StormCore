#include "ShotgunReticle.h"

#include "Components/CanvasPanelSlot.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/CadenceShotgun/CadenceShotgunActor.h"
#include "RobotHunter/Utils/ScreenUtils.h"

void UShotgunReticle::InitWithDelay()
{
	ARightArmActor* _arm = GetRightMember();
	CHECK_NULL(_arm, "Right arm is null");

	shotgun = Cast<ACadenceShotgunActor>(_arm);
	CHECK_NULL(shotgun, "shotgun is null");

	SetMinMaxPercent(shotgun->GetRhythmMeasureMin(), shotgun->GetRhythmMeasureMax());
	Super::InitWithDelay();
}

void UShotgunReticle::RegisterEvents()
{
	//get ratio
	BIND_ACTION(shotgun, OnReductionUpdate, UShotgunReticle, UpdateCircleBeat);
	BIND_ACTION(shotgun, OnNumberOfStacksChange, UShotgunReticle, UpdateStack);
	Super::RegisterEvents();
}

void UShotgunReticle::NativePreConstruct()
{
	Super::NativePreConstruct();
	UpdateSizeCircle(FVector2D(settingsCircleBeat.maxSize));
	UpdateColorCircleBeat(settingsCircleBeat.circleBeatColor);
	UpdateColorCircleReticle(settingsCircle.circleColor);
	UpdateThicknessCircleBeat(settingsCircleBeat.thicknessCircle);
	SET_TEXT_STR(textStack, "0");
	if (useDebug)
	{
		UpdateCircleBeat(ratioBeat / 100.f);
		SetMinMaxPercent(DebugRatioMin, DebugRatioMax);
	}
	else
	{
		UpdateCircleBeat(0.f);
		SetMinMaxPercent(0, 100);
	}
}

void UShotgunReticle::UpdateCircleBeat(const float _ratio)
{
	UpdateSizeCircle(FVector2D(settingsCircleBeat.maxSize));
	CHECK_NULL(circleBeat, "Circle beat is null");
	TObjectPtr<UCanvasPanelSlot> _slot = Cast<UCanvasPanelSlot>(circleBeat->Slot);
	CHECK_NULL(_slot, "Slot is null");

	const FVector2D _size = FVector2D(settingsCircleBeat.maxSize * (1.f - _ratio));

	const FVector2D _screen = UScreenUtils::GetScreenViewport();
	const float _marginRL = UScreenUtils::GetMargin(_screen, _size, EMarginSide::Left);
	const float _marginTB = UScreenUtils::GetMargin(_screen, _size, EMarginSide::Top);
	const FAnchors _anchors = UScreenUtils::GetAnchorsTargetZone(_screen, _marginRL, _marginTB);

	_slot->SetAnchors(_anchors);
	_slot->SetOffsets(FMargin(0));
	circleBeat->SetThickness(settingsCircleBeat.thicknessCircle);
}


void UShotgunReticle::UpdateStack(int _stack)
{
	SET_TEXT_STR(textStack, INTSTR(_stack));
}

void UShotgunReticle::UpdateSizeCircle(const FVector2D& _size) const
{
	CHECK_NULL(circleReticle, "Circle reticle is null");
	TObjectPtr<UCanvasPanelSlot> _slot = Cast<UCanvasPanelSlot>(circleReticle->Slot);
	CHECK_NULL(_slot, "Slot is null");

	const FVector2D _screen = UScreenUtils::GetScreenViewport();
	const float _marginRL = UScreenUtils::GetMargin(_screen, _size, EMarginSide::Left);
	const float _marginTB = UScreenUtils::GetMargin(_screen, _size, EMarginSide::Top);
	const FAnchors _anchors = UScreenUtils::GetAnchorsTargetZone(_screen, _marginRL, _marginTB);

	_slot->SetAnchors(_anchors);
	_slot->SetOffsets(FMargin(0));
}

void UShotgunReticle::UpdateColorCircleBeat(const FLinearColor& _color) const
{
	circleBeat->SetColor(_color);
}

void UShotgunReticle::UpdateColorCircleReticle(const FLinearColor& _color) const
{
	circleReticle->SetColor(_color);
}

#if WITH_EDITOR

void UShotgunReticle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FString StrLower = PropertyName.ToString().ToLower();

	TMap<FString, TFunction<void()>> Events =
	{
		{
			"thicknessCircle", [this]
			{
				UpdateThicknessCircleBeat(settingsCircleBeat.thicknessCircle);
			}
		},
		{
			"maxSize", [this]
			{
				UpdateCircleBeat(0.f);
				UpdateSizeCircle(FVector2D(settingsCircleBeat.maxSize));
			}
		},
		{
			"circleBeatColor", [this]
			{
				UpdateColorCircleBeat(settingsCircleBeat.circleBeatColor);
			}
		},
		{
			"circleColor", [this]
			{
				UpdateColorCircleReticle(settingsCircle.circleColor);
			}
		},
		{
			"useDebug", [this]
			{
				DebugRatioMin = 0.f;
				DebugRatioMax = 100.f;
				ratioBeat = 0.f;
				UpdateCircleBeat(0.f);
			}
		},
		{
			"debugRatioMin", [this]
			{
				if (DebugRatioMin > DebugRatioMax)
				{
					DebugRatioMin = DebugRatioMax - 1.f;
				}
				SetMinMaxPercent(DebugRatioMin, DebugRatioMax);
			}
		},
		{
			"debugRatioMax", [this]
			{
				if (DebugRatioMax < DebugRatioMin)
				{
					DebugRatioMax = DebugRatioMin + 1.f;
				}
				SetMinMaxPercent(DebugRatioMin, DebugRatioMax);
			}
		},
		{
			"ratioBeat", [this]
			{
				UpdateCircleBeat(ratioBeat / 100.f);
			}
		},

	};

	if (Events.Contains(StrLower))
	{
		Events[StrLower]();
	}
}
#endif


void UShotgunReticle::SetMinMaxPercent(const float _ratioMin, const float _ratioMax, const bool _inverse)
{
	CHECK_NULL(circleReticle, "Circle reticle is null");
	circleReticle->SetPercentMin(_inverse ? 1 - _ratioMin : _ratioMin);
	circleReticle->SetPercentMax(_inverse ? 1 - _ratioMax : _ratioMax);
}

void UShotgunReticle::UpdateThicknessCircleBeat(const float _thickness) const
{
	circleBeat->SetThickness(_thickness);
}
