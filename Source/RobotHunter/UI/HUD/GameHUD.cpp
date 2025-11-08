#include "GameHUD.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"

TArray<FPositionViewing> AGameHUD::GetLocationsViewingPoints(EReticleType _reticleType) const
{
	if (currentUI != EStateUI::PlayerGameUI || _reticleType == EReticleType::None || !reticles[_reticleType])
	{
		return {FPositionViewing()};
	}

	return reticles[_reticleType]->GetLocationsViewingPoints();
}

FPositionViewing AGameHUD::GetCenterPoint() const
{
	if (currentUI != EStateUI::PlayerGameUI)
	{
		return FPositionViewing();
	}

	const TObjectPtr<APlayerController> _fpc = GetWorld()->GetFirstPlayerController();

	if (!_fpc)
	{
		return FPositionViewing();
	}

	int ViewportSizeX, ViewportSizeY;
	_fpc->GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Deproject screen point
	FVector ShotStart, ShotDirection;
	_fpc->DeprojectScreenPositionToWorld(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f, ShotStart, ShotDirection);
	// Assuming crosshair is in screen center

	return FPositionViewing(ShotStart, ShotDirection);
}

void AGameHUD::HideUI()
{
	Super::HideUI();
}

void AGameHUD::ShowUI()
{
	Super::ShowUI();
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
}
