// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/InputKeySelector.h"
#include "Components/SizeBox.h"
#include "Components/WidgetSwitcher.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "RobotHunter/UI/Template/Prefab/TooltipInput/PrefabTooltipInput.h"
#include "KeyInput.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UKeyInput : public UPrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textPrefab;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWidgetSwitcher> ControllerSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UInputKeySelector> KeySelector;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USizeBox> sizeBoxWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> gamepadImage;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatText;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FKey KeySelected;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText Text;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UTexture2D> textureGamepad;

	UPROPERTY(EditAnywhere, Category = "Custom", meta = (ClampMin=0, UIMin= 0))
	int32 size;

public:
	virtual void NativePreConstruct() override;

	void SwitchToController() const;
	void SwitchToKeyboard() const;
	void InitWidget(const FKey& Key, const FText& Text, const FString& String, const TObjectPtr<UTexture2D>& Object);
};
