// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "RobotHunter/UI/ATH/Reticle/Reticle.h"
#include "RobotHunter/UI/Template/Prefab/Radial/CustomRadialThickness.h"
#include "RobotHunter/UI/Template/Prefab/Radial/CustomRadialDonut.h"
#include "ShotgunReticle.generated.h"

class ACadenceShotgunActor;

USTRUCT()
struct FSettingsCircleBeat
{
	GENERATED_BODY()

	// The size of the gauge in pixel in maximum (ratio 0 of shoot)
	UPROPERTY(EditAnywhere)
	float maxSize = 450;

	//The thickness of circle beat in percent (100% = maximum size)
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.01f, UIMin = 0.01f, ClampMax = 1, UIMax = 1))
	float thicknessCircle = 1.f;

	//The current color of circle beat
	UPROPERTY(EditAnywhere)
	FLinearColor circleBeatColor = FLinearColor::White;
};

USTRUCT()
struct FSettingsCircle
{
	GENERATED_BODY()

	//The current color of circle beat
	UPROPERTY(EditAnywhere)
	FLinearColor circleColor = FLinearColor::White;
};

#define DEBUG_SHOTGUN_CROSSHAIR true
/**
 * A class representing the crosshair for a shotgun, responsible for updating and managing
 * the UI reticles based on shotgun behavior and properties. Inherits from UReticle.
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UShotgunReticle final : public UReticle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCustomRadialDonut> circleReticle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCustomRadialThickness> circleBeat;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> textStack;

	UPROPERTY()
	TObjectPtr<ACadenceShotgunActor> shotgun;

	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	FSettingsCircleBeat settingsCircleBeat;

	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	FSettingsCircle settingsCircle;

	UPROPERTY(EditAnywhere, Category = "Custom|Debug")
	bool useDebug = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Debug",
		meta = (Delta = 1, ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100, Units="Percent", AllowPrivateAccess,
			EditCondition="useDebug", EditConditionHides))
	float DebugRatioMin = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Debug",
		meta = (Delta = 1, ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100, Units="Percent", AllowPrivateAccess,
			EditCondition="useDebug", EditConditionHides))
	float DebugRatioMax = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom|Debug",
		meta = (ClampMin = 0, UIMin = 0, ClampMax = 100, UIMax = 100, Units="Percent", AllowPrivateAccess,
			EditCondition="useDebug", EditConditionHides))
	float ratioBeat = 0.f;

public:
	/**
	 * Set the minimum of percent from weapon and inverse value to convert in UI
	 * @param _ratioMin
	 * @param _ratioMax ratio (0 = externe crosshair, 1 = interior)
	 * @param _inverse Set if inverse
	 */
	UFUNCTION(BlueprintCallable)
	void SetMinMaxPercent(float _ratioMin, float _ratioMax, const bool _inverse = false);

private:
	virtual void InitWithDelay() override;
	virtual void RegisterEvents() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void UpdateCircleBeat(const float _ratio);
	UFUNCTION()
	void UpdateStack(int _stack);

	void UpdateSizeCircle(const FVector2D& _size) const;
	void UpdateThicknessCircleBeat(const float _thickness) const;
	void UpdateColorCircleBeat(const FLinearColor& _color) const;
	void UpdateColorCircleReticle(const FLinearColor& _color) const;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
};
