// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/UI.h"
#include "Components/RadialSlider.h"
#include "DraisineATH.generated.h"


USTRUCT()
struct FSliderInformation
{
	GENERATED_BODY()

	//The angle of start circle
	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0f, UIMax = 360.0f, ClampMin = 0.0f, ClampMax = 360.0f))
	float sliderStartAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	bool syncAngle = true;

	//The angle of end circle
	UPROPERTY(EditAnywhere,
		meta = (UIMin = 0.0f, UIMax = 360.0f, ClampMin = 0.0f, ClampMax = 360.0f, EditCondition = "!syncAngle"))
	float sliderEndAngle = 360.0f;

	//The angle of rotation slider
	UPROPERTY(EditAnywhere, meta = (UIMin = -180.0f, UIMax = 180.0f, ClampMin = -180.0f, ClampMax = 180.0f))
	float angleRotation = 0.0f;

	FSliderInformation()
	{
	}

	FSliderInformation(const float SliderStartAngle, const float SliderEndAngle, const float AngleRotation)
		: sliderStartAngle(SliderStartAngle), sliderEndAngle(SliderEndAngle), angleRotation(AngleRotation)
	{
	}
};

UENUM()
enum ESliderColor
{
	BlackSlider,
	RedSlider,
	YellowSlider,
	GreenSlider,

	SliderCount
};

/**
 *
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UDraisineATH final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> blackRadialSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> redRadialSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> yellowRadialSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> greenRadialSlider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<URadialSlider> progressSlider;

	//Override the settings of black slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (InlineEditConditionToggle))
	bool overrideBlackSliderValue = true;

	//Initial parameter of slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (EditCondition = "overrideBlackSliderValue"))
	FSliderInformation blackSliderInformation;

	//Override the settings of red slider
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle))
	bool overrideRedSliderValue = true;
	//Initial parameter of slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (EditCondition = "overrideRedSliderValue"))
	FSliderInformation redSliderInformation;

	//Override the settings of yellow slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (InlineEditConditionToggle))
	bool overrideYellowSliderValue = true;
	//Initial parameter of slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (EditCondition = "overrideYellowSliderValue"))
	FSliderInformation yellowSliderInformation;

	//Override the settings of green slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (InlineEditConditionToggle))
	bool overrideGreenSliderValue = true;
	//Initial parameter of slider
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (EditCondition = "overrideGreenSliderValue"))
	FSliderInformation greenSliderInformation;

public:
	FORCEINLINE float GetProgressAngularOffset() const { return progressSlider->AngularOffset; }
	FORCEINLINE float GetSliderAngularOffset(const ESliderColor& _color) const
	{
		URadialSlider* _slider = GetSliderFromEnum(_color);

		if (!_slider)
		{
			return 0.0f;
		}

		return _slider->AngularOffset;
	}

	/**
	 * Set the rotation angle of green and yellow radial
	 * @param _angleRotation value of angle (It will be clamped between 0 and 360, only if angle > 360)
	 */
	void SetRotationAngleSync(const float _angleRotation);

	/**
	 *  Set the rotation angle of slider color
	 * @param _slider Enum of slider color [Edited by Giusy]
	 * @param _angleRotation value of angle (It will be clamped between 0 and 360, only if angle > 360)
	 */
	void SetRotationAngle(const ESliderColor _slider, const float _angleRotation);

	/**
	 * Set the angle of the slider to be synchronized with start and end so that they are equal
	 * @param _slider Enum of slider color
	 * @param _sync Boolean to set if start and end will be equal in size
	 * @param _sizeAngleStart value of angle of start (left) (It will be clamped between 0 and 360)
	 * @param _sizeAngleEnd value of angle of end (right) (It will be clamped between 0 and 360).\n
	 * By default, the end angle size is 0, but will be replaced if synchronization is active.
	 */
	void SetAngle(ESliderColor _slider, const bool _sync, float _sizeAngleStart, float _sizeAngleEnd = 0) const;

	/**
	 * Set the value of progress in slider
	 * @param _value A value of slider (It will be clamped between 0 and 1 of slider)
	 */
	void SetValueProgress(float _value) const;

	/**
	 * Set the angular offset of the progress slider [Added by Giusy]
	 * @param _angleRotation Rotation of angular offset (It will be clamped between the MinSlider and the MaxSlider of the black slider)
	 */
	void SetProgressAngularOffset(float _angleRotation);

	/**
	* Set the color of the handle in the progress slider [Added by Giusy]
	* @param _color Enum of slider color
	*/
	void SetProgressColor(const ESliderColor& _color);

	/**
	 * Get the minimum angle of slider to set correct value
	 * @return Minimum angle between each slider
	 */
	float GetMinSlider() const;

	/**
	 * Get the minimum angle of a slider [Added by Giusy]
	 * @param _color Enum of slider color
	 * @return Minimum angle between each slider
	 */
	float GetMinSlider(const ESliderColor& _color) const;

	/**
	 * Get the maximum angle of slider to set correct value
	 * @return maximum angle between each slider
	 */
	float GetMaxSlider() const;

	/**
	 * Get the maximum angle of a slider [Added by Giusy]
	 * @param _color Enum of slider color
	 * @return maximum angle between each slider
	 */
	float GetMaxSlider(const ESliderColor& _color) const;

#pragma region private

private:
	/**
	 * Get the slider from enum
	 * @param _radialSlider Enum of slider color
	 * @return pointer of radial slider
	 */
	FORCEINLINE TObjectPtr<URadialSlider> GetSliderFromEnum(ESliderColor _radialSlider) const
	{
		switch (_radialSlider)
		{
		case BlackSlider: return blackRadialSlider;
		case RedSlider: return redRadialSlider;
		case YellowSlider: return yellowRadialSlider;
		case GreenSlider: return greenRadialSlider;
		default: return nullptr;
		}
	}

	virtual void NativePreConstruct() override;

	/**
	 * Set the rotation angle of slider, with parameter of sync between yellow and green slider
	 * @param _sync boolean to synchronize yellow and green slider
	 * @param _angleRotation angle of rotation
	 * @param _sliderColor sliderColor if sync not activated (default is ESliderColor()) [Edited by Giusy]
	 */
	void SetRotationAngleValue(const bool _sync, float _angleRotation,
	                           const ESliderColor _sliderColor = ESliderColor());

	/**
	 * Initialize slider on preconstruct of widget
	 * @param _radialSlider radial slider pointer
	 * @param _information information struct of slider
	 */
	void InitializeRadialSlider(const TObjectPtr<URadialSlider>& _radialSlider, FSliderInformation& _information) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion private
};
