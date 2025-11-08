// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFliesReticle.h"

#include "Components/CanvasPanelSlot.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/Fireflies/FirefliesActor.h"
#include "RobotHunter/Utils/ScreenUtils.h"


void UFirefliesCrosshair::RegisterEvents()
{
	Super::RegisterEvents();

	BIND_ACTION(fireflies, OnFirefliesShoot, UFirefliesCrosshair, OverheatIncrease);
	BIND_ACTION(fireflies, OnMark, UFirefliesCrosshair, SetTextMarkedEnemy);
	if (AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter()))
		BIND_ACTION(_chara, OnScope, UFirefliesCrosshair, SetScoping);

	FViewport::ViewportResizedEvent.AddUObject(this, &UFirefliesCrosshair::Resize);
	GEngine->GameViewport->OnToggleFullscreen().AddUObject(this, &UFirefliesCrosshair::ResizeFullScreen);
}

void UFirefliesCrosshair::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetSizeOverlay(GetSizeFromPercent(settingsFireflies.percentageInactive), true);
	SetColor(0);
	//SetOpacity(settingsFireflies.opacityMin, PARAM_OPACITY_MIN_PERCENT);
	//SetOpacity(settingsFireflies.opacityMax, PARAM_OPACITY_MAX_PERCENT);
	SET_TEXT_STR(MarkedTarget->GetText(), FORMAT(settingsFireflies.formatMarked, 0));

	if (debug)
	{
		//SetPercent(percentOverheat, PARAM_PERCENT);
		SetSizeOverlay(GetSizeFromPercent(percentOverheat), true);
	}
	else
	{
		//SetPercent(0, PARAM_PERCENT);
		percentOverheat = 0;
	}
}

#if WITH_EDITOR
void UFirefliesCrosshair::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FString PropertyLower = PropertyName.ToString().ToLower();

	// Map property names to their corresponding functions/parameters
	const TMap<FString, TFunction<void()>> PropertyActions = {
		{"percentstart", [this] { SetSizeOverlay(FVector2D(GetSizeFromPercent(0.f)), true); }},
		{"percentend", [this] { SetSizeOverlay(FVector2D(GetSizeFromPercent(100.f)), true); }},
		{"colornormal", [this] { SetColor(percentOverheat); }},
		{"coloroverheat", [this] { SetColor(percentOverheat); }},
		{
			"percentoverheat", [this]
			{
				//SetPercent(percentOverheat, PARAM_PERCENT);
				SetColor(percentOverheat);
				SetSizeOverlay(GetSizeFromPercent(percentOverheat), true);
			}
		},
		//{"opacitymin", [this] { SetOpacity(settingsFireflies.opacityMin, PARAM_OPACITY_MIN_PERCENT); }},
		//{"opacitymax", [this] { SetOpacity(settingsFireflies.opacityMax, PARAM_OPACITY_MAX_PERCENT); }}
	};

	// Execute corresponding action if the property exists in the map
	if (PropertyActions.Contains(PropertyLower))
	{
		PropertyActions[PropertyLower]();
	}
}
#endif


void UFirefliesCrosshair::SetSizeOverlay(const FVector2D _size, const bool _needApplied)
{
	const FVector2D _screen = UScreenUtils::GetScreenViewport();
	const float _marginLR = UScreenUtils::GetMargin(_screen, _size, EMarginSide::Left);
	const float _marginTB = UScreenUtils::GetMargin(_screen, _size, EMarginSide::Top);

	TObjectPtr<UCanvasPanelSlot> _slot = Cast<UCanvasPanelSlot>(overlay->Slot);
	CHECK_NULL(_slot, "Slot is null");

	_slot->SetAnchors(UScreenUtils::GetAnchorsTargetZone(_screen, _marginLR, _marginTB));
	_slot->SetOffsets(FMargin(0));

	if (_needApplied)
	{
		screenResolution = _size;
	}
}

void UFirefliesCrosshair::ResizeFullScreen(bool bArg)
{
	Resize(nullptr, bArg);
}


FVector2D UFirefliesCrosshair::GetSizeFromPercent(const float _percent) const
{
	const FVector2D _screen = UScreenUtils::GetScreenViewport();
	const FVector2D _sizeNormal = _screen * (settingsFireflies.percentageStart / 100.f);
	const FVector2D _sizeOverheat = _screen * (settingsFireflies.percentageEnd / 100.f);

	return FMath::Lerp(_sizeNormal, _sizeOverheat, _percent / 100.f);
}

bool UFirefliesCrosshair::IsActorInTargetZone(const TObjectPtr<AActor>& _actor) const
{
	if (!GetWorld() || !GETFPC) return false;
	APlayerController* _controller = GETFPC;
	CHECK_NULL_WITH_RETURN(_controller, "Controller is null", return false);
	FVector2D _result;
	UGameplayStatics::ProjectWorldToScreen(_controller, _actor->GetActorLocation(), _result, true);

	return UScreenUtils::VectorContainsInTargetZone(_result, screenResolution);
}

void UFirefliesCrosshair::RemoveActorDetection(const TObjectPtr<AActor>& _actor)
{
	//Removed
	/*if (widgetComponentsOfActors.Contains(_actor))
	{
		widgetComponentsOfActors.Remove(_actor);
	}*/
}

void UFirefliesCrosshair::SetScoping(bool _scoping)
{
	//overlay->SetVisibility(_scoping ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (!_scoping)
	{
		const FVector2D _screen = UScreenUtils::GetScreenViewport();
		SetSizeOverlay(_screen * (settingsFireflies.percentageInactive / 100.0f), true);
	}
	else
		SetSizeOverlay(GetSizeFromPercent(fireflies->GetCurrentOverheat()), true);
}

void UFirefliesCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UFirefliesCrosshair::OverheatIncrease(const float _percentageOverheat)
{
	if (fireflies->GetIsScoped())
		SetSizeOverlay(GetSizeFromPercent(_percentageOverheat), true);
	//SetPercent(_percentageOverheat, PARAM_PERCENT);
}

void UFirefliesCrosshair::SetTextMarkedEnemy(const int _marked, AActor* _actorMark)
{
	SET_TEXT_STR(MarkedTarget->GetText(), FORMAT(settingsFireflies.formatMarked, _marked));
}


void UFirefliesCrosshair::SetColor(const float _percent) const
{
	CHECK_NULL(crosshairZone, "crosshair zone is null");
	CHECK_NULL(crosshairZone->GetDynamicMaterial(), "null");
	//crosshairZone->GetDynamicMaterial()->SetVectorParameterValue(_parameterValue, _color);
	FSlateBrush _slate = crosshairZone->GetBrush();
	_slate.TintColor = FMath::Lerp(settingsFireflies.colorNormal, settingsFireflies.colorOverheat, _percent / 100.f);
	//LOG_DEBUG(FLOATSTR(_percent / 100.f));
	crosshairZone->SetBrush(_slate);
}

/*
void UFirefliesCrosshair::SetPercent(const float _percent, const FName& _parameterValue)
{
	CHECK_NULL(crosshairZone, "crosshair zone is null");
	CHECK_NULL(crosshairZone->GetDynamicMaterial(), "null");
	c//rosshairZone->GetDynamicMaterial()->SetScalarParameterValue(_parameterValue, _percent);
}

void UFirefliesCrosshair::SetOpacity(const float _opacity, const FName& _parameterValue) const
{
	CHECK_NULL(crosshairZone, "crosshair zone is null");
	CHECK_NULL(crosshairZone->GetDynamicMaterial(), "null");
	//crosshairZone->GetDynamicMaterial()->SetScalarParameterValue(_parameterValue, _opacity);
}*/

void UFirefliesCrosshair::Resize(FViewport* ViewPort, uint32 val)
{
	if (!fireflies) return;
	if (!fireflies->GetIsScoped())
	{
		const FVector2D _screen = UScreenUtils::GetScreenViewport();
		SetSizeOverlay(_screen * (settingsFireflies.percentageInactive / 100.0f), true);
	}
	else
		SetSizeOverlay(GetSizeFromPercent(fireflies->GetCurrentOverheat()), true);
}

void UFirefliesCrosshair::InitWithDelay()
{
	ARightArmActor* _arm = GetRightMember();
	CHECK_NULL(_arm, "Right arm is null");

	fireflies = Cast<AFirefliesActor>(_arm);
	CHECK_NULL(fireflies, "fireflies is null");

	SetTextMarkedEnemy(fireflies->GetCurrentMark(), nullptr);

	fireflies->SetReticle(this);
	debug = false;

	const FVector2D _screen = UScreenUtils::GetScreenViewport();
	SetSizeOverlay(_screen * (settingsFireflies.percentageInactive / 100.0f), true);
	//SetPercent(0, PARAM_PERCENT);
	percentOverheat = 0;
	debug = false;

	playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	//overlay->SetVisibility(ESlateVisibility::Hidden);

	Super::InitWithDelay();
}
