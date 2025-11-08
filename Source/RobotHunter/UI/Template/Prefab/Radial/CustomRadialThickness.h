// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CustomRadialThickness.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomRadialThickness final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> imageRoundBar;

	//The thickness in percent normalized of circle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom",
		meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1", AllowPrivateAccess))
	float Thickness = 0.5f;

	//The color of circle	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	FLinearColor Color = FLinearColor::White;

public:
	/**
	 * Set the thickness of circle
	 * @param _thickness Thickness of round
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetThickness(const float _thickness);

	/**
	 * Set the color of circle
	 * @param _color Color of circle
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void SetColor(const FLinearColor& _color);
};
