// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CustomRadialDonut.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomRadialDonut final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> imageRoundBar;

	//The thickness in percent normalized of circle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom",
		meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100", AllowPrivateAccess))
	float percentMin = 0.f;
	//The thickness in percent normalized of circle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom",
		meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100", AllowPrivateAccess))
	float percentMax = 100.f;

	//The color of circle	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	FLinearColor Color = FLinearColor::White;

public:
	/**
	 * Set the thickness of circle
	 * @param _percentMin Percentage minimum (100 is center)
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetPercentMin(const float _percentMin);

	/**
	 * Set the thickness of circle
	 * @param _percentMax Percentage maximum (100 is center)
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetPercentMax(const float _percentMax);

	/**
	 * Set the color of circle
	 * @param _color Color of circle
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetColor(const FLinearColor& _color);
};
