#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"
#include "WDG_MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UWDG_MiniMap final : public UBaseMenu
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> MiniMapCaptureImg;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> Map3D_map3DBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> Map3D_GameBtn;

#pragma region Getter/Setter

public:
	UFUNCTION()
	void ShowGamePause();
	virtual void RegisterEvents() override;
	/**
	 * Called each time showed
	 */
	virtual void Refresh() override;

	virtual void ExitUI() override;

	TObjectPtr<UImage> GetMiniMapCaptureImg() const { return MiniMapCaptureImg; }
#pragma endregion
};
