// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Pause/PauseFrame.h"
#include "MapUI.generated.h"

class UHintInformation;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UMapUI final : public UPauseFrame
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<TEnumAsByte<EObjectTypeQuery>> layersToHit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHintInformation> HintInformation;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FAnchors sizeHintInformation;

	//Offset in X axis of tooltip
	//Between -1 and 1 (position of anchors)
	//Need to restart engine to apply
	UPROPERTY(EditAnywhere, Category = "Custom|Offset",
		meta = (ClampMin = -1.0f, ClampMax = 1.0f, UIMin = -1.0f, UIMax = 1.0f))
	float offsetX = 2.0f;
	//Offset in Y axis of tooltip
	//Between -1 and 1 (position of anchors)
	//Need to restart engine to apply
	UPROPERTY(EditAnywhere, Category = "Custom|Offset",
		meta = (ClampMin = -1.0f, ClampMax = 1.0f, UIMin = -1.0f, UIMax = 1.0f))
	float offsetY = -0.3f;

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FAnchors AddAnchors(const FAnchors& _a, const FAnchors& _b) const;
	void AddOffsetX(FAnchors& _a, const float& _offset) const;
	void AddOffsetY(FAnchors& _a, const float& _offset) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
};
