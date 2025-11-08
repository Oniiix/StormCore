// Fill out your copyright notice in the Description page of Project Settings.


#include "Reticle.h"

#include "BaseReticle/BaseReticle.h"
#include "Components/CanvasPanelSlot.h"
#include "RobotHunter/UI/HUD/GameHUD.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/RightArmActor.h"


TArray<FPositionViewing> UReticle::GetLocationsViewingPoints() const
{
	if (PercentCrosshairInScreen.IsEmpty())
	{
		LOG_ERROR(true, "The viewport postion is empty.");
		return {FPositionViewing()};
	}

	const EStateUI currentUI = hud->GetState();
	if (currentUI == EStateUI::None)
	{
		return {FPositionViewing()};
	}

	if (currentUI == EStateUI::PlayerGameUI)
	{
		const APlayerController* _fpc = GetWorld()->GetFirstPlayerController();

		CHECK_NULL_WITH_RETURN(_fpc, "FPC is null", return {FPositionViewing()});

		int ViewportSizeX, ViewportSizeY;
		_fpc->GetViewportSize(ViewportSizeX, ViewportSizeY);


		FVector ShotStart, ShotDirection;
		_fpc->DeprojectScreenPositionToWorld(ViewportSizeX * .5f, ViewportSizeY * .5f, ShotStart, ShotDirection);


		return {FPositionViewing(ShotStart, ShotDirection)};
	}
	return {FPositionViewing()};
}


void UReticle::UpdatePositionAnchors(FVector2D& _position, const TObjectPtr<UImage>& _crosshair)
{
	if (UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(_crosshair->Slot))
	{
		const FAnchors _anchors = _slot->GetAnchors();
		_position.X = _anchors.Minimum.X;
		_position.Y = _anchors.Minimum.Y;
	}
}


TObjectPtr<ALeftArmActor> UReticle::GetLeftMember() const
{
	const TObjectPtr<AMainCharacter> _chara = Cast<AMainCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());
	CHECK_NULL_WITH_RETURN(_chara, "Character is null", return nullptr);

	return _chara->GetUsingLeftArm();
}

void UReticle::NativePreConstruct()
{
	Super::NativePreConstruct();

	//Resize();
}

void UReticle::RegisterEvents()
{
	Super::RegisterEvents();

	//TODO remove after play test
	//FViewport::ViewportResizedEvent.AddUObject(this, &UReticle::Resize);
	//GEngine->GameViewport->OnToggleFullscreen().AddUObject(this, &UReticle::ResizeFullScreen);
}

void UReticle::ResizeFullScreen(bool bArg) const
{
	//Resize();
}

void UReticle::Resize(FViewport* Viewport, unsigned I) const
{
	/*TObjectPtr<UCanvasPanelSlot> _slot = Cast<UCanvasPanelSlot>(baseReticle->Slot);
	CHECK_NULL(_slot, "_Slot is null");
	
	const FVector2D _screen = UScreenUtils::GetScreenViewport();
	const FVector2D _sizeReticle = FVector2D(baseReticle->GetSizeReticle());
	const float _marginRL = UScreenUtils::GetMargin(_screen, _sizeReticle, EMarginSide::Left);
	const float _marginTB = UScreenUtils::GetMargin(_screen, _sizeReticle, EMarginSide::Top);
	
	const FAnchors _anchors = UScreenUtils::GetAnchorsTargetZone(_screen, _marginRL, _marginTB);
	_slot->SetAnchors(_anchors);
	_slot->SetOffsets(FMargin(0));*/
}

TObjectPtr<ARightArmActor> UReticle::GetRightMember() const
{
	const TObjectPtr<AMainCharacter> _chara = Cast<AMainCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());
	CHECK_NULL_WITH_RETURN(_chara, "Character is null", return nullptr);

	return _chara->GetUsingRightArm();
}
