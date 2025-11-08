// 

#pragma once

#include "CoreMinimal.h"
#include "Components/HorizontalBox.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "PrefabTooltipInput.generated.h"

class UKeyInput;

USTRUCT()
struct FKeyTooltip
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FKey key;

	UPROPERTY(EditAnywhere)
	FText name;

	UPROPERTY(EditAnywhere)
	FString format;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> imageGamepadKey;
};

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UPrefabTooltipInput : public UPrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> hbTooltipInput;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<FKeyTooltip> keys;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UKeyInput> referenceKeyInput;

	UPROPERTY()
	TArray<UKeyInput*> CachedInputWidgets;

	bool isGamepad = false;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
