// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/UI/UI.h"
#include "DayInformation.generated.h"

class UTextPrefab;
class ATimeManager;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FSettingsCircleFilled
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FLinearColor Color = FLinearColor::White;

	//Angle offset (rotation inverse clock)
	UPROPERTY(EditAnywhere, meta = (UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float angleOffset = 0.f;

	//Size of circle
	UPROPERTY(EditAnywhere, meta = (UIMin=0, UIMax=0.5, ClampMin=0, ClampMax=0.5))
	float radius = 0.5f;

	//Thickness of circle
	UPROPERTY(EditAnywhere, meta = (UIMin=0, UIMax=0.5, ClampMin=0, ClampMax=0.5))
	float thickness = 0.1f;

	//The maximum of circle filled
	UPROPERTY(EditAnywhere,
		meta = (EditCondition = "useDebug", EditConditionHides, UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float maxFill = 1.f;
};

UCLASS()
class ROBOTHUNTER_API UDayInformation final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textDayWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Custom|Assets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> circleEmpty;

	UPROPERTY(BlueprintReadWrite, Category = "Custom|Assets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> circleFill;

	//Settings of circle empty
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Settings", meta = (AllowPrivateAccess))
	FSettingsCircleFilled settingsCircleEmpty;

	//Settings of filled circle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Settings", meta = (AllowPrivateAccess))
	FSettingsCircleFilled settingsCircleFill;

	//Percentage of slider to change color
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float percentChangeColor = 0.8f;

	//Active this boolean if you want test fill of circle
	UPROPERTY(EditAnywhere, Category = "Custom|Debug")
	bool useDebug = false;

	//Test for filling circle
	UPROPERTY(EditAnywhere, Category = "Custom|Debug",
		meta = (EditCondition = "useDebug", EditConditionHides, UIMin=0, UIMax=1, ClampMin=0, ClampMax=1))
	float fill = 0.f;

	//The text formated show in UI
	UPROPERTY(EditAnywhere, Category = "Custom|Bar")
	FString stringToShow = "{0}:{1}";

	//This color showed after percentage of day passed
	UPROPERTY(EditAnywhere, Category = "Custom|Bar")
	FLinearColor colorWarning = FLinearColor::Red;

	//This color showed of normal text
	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	FLinearColor colorNormal = FLinearColor::White;

	//This color showed of warning text
	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	FLinearColor colorWarningText = FLinearColor::Red;

	UPROPERTY(EditAnywhere, Category = "Custom|Text")
	FString format = "<normal>{0}</>";

	UPROPERTY()
	ATimeManager* timeManager;

	UFUNCTION()
	void UpdateTime(const int Hour, const int Minute);


	/**
	 * Change the color of image
	 * @param _image Image circle
	 * @param settingsCircle settings
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ChangeColor(UImage* _image, const FSettingsCircleFilled& settingsCircle);

	/**
	 * Change the color of text
	 * @param _color color
	 */
	void ChangeColorText(const FLinearColor& _color) const;

	virtual void NativePreConstruct() override;
	virtual void Init() override;
	virtual void InitWithDelay() override;
	virtual void RegisterEvents() override;

	/**
	 * Fill the circle from a percentage
	 * @param _circle Cirle image
	 * @param _percent Percentage to fill (between 0 and 100)
	 */
	void SetCircleFilled(UImage* _circle, const float _percent) const;

	/**
	 * Change the color of image
	 * @param _image Image circle
	 * @param color color
	 */
	void ChangeColor(UImage* _image, const FLinearColor& color);

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
