// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "RobotHunter/UI/ATH/Reticle/Reticle.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "FirefliesReticle.generated.h"

class AFirefliesActor;

#define PARAM_COLOR_MIN_PERCENT "Color Min Percent"
#define PARAM_COLOR_MAX_PERCENT "Color Max Percent"
#define PARAM_OPACITY_MIN_PERCENT "Opacity Min Fill"
#define PARAM_OPACITY_MAX_PERCENT "Opacity Max Fill"
#define PARAM_PERCENT "Percent"

USTRUCT()
struct FSettings
{
	GENERATED_BODY()

	//Percentage from screen viewport of start reticule (based on 1920x1080 in editor)
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100, Units="Percent"))
	float percentageStart = 75.f;

	//Percentage from screen viewport of end reticule (based on 1920x1080 in editor)
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100, Units="Percent"))
	float percentageEnd = 20.f;

	//Percentage from screen viewport of end reticule (based on 1920x1080 in editor)
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100, Units="Percent"))
	float percentageInactive = 10.0f;

	//Color of reticle normal
	UPROPERTY(EditAnywhere)
	FLinearColor colorNormal = FLinearColor::White;

	//Color of overheated reticle
	UPROPERTY(EditAnywhere)
	FLinearColor colorOverheat = FLinearColor::Red;

	//Opacity minimum of filled zone
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100, Units="Percent"))
	float opacityMin = 0;

	//Opacity maximum of filled zone
	UPROPERTY(EditAnywhere, meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100, Units="Percent"))
	float opacityMax = 100;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatMarked = "<normal>{0}</>";
};

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UFirefliesCrosshair final : public UReticle
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AFirefliesActor> fireflies;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> crosshairZone;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCanvasPanel> overlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> MarkedTarget;

	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	FSettings settingsFireflies;

	//Check this if you want use debug (reset in game)
	UPROPERTY(EditAnywhere, Category = "Custom|Settings|Debug")
	bool debug = false;

	//Percentage of overheated
	UPROPERTY(EditAnywhere, Category = "Custom|Settings|Debug",
		meta = (UIMin = 0, UIMax = 100, ClampMin = 0, ClampMax = 100, EditCondition = "debug", EditConditionHides, Units
			="Percent"))
	float percentOverheat = false;

	//Screen size of zone
	UPROPERTY(VisibleAnywhere, Category = "Custom|Settings|Debug", meta = (EditCondition = "debug", EditConditionHides))
	FVector2D screenResolution = FVector2D(0);

	UPROPERTY()
	APawn* playerPawn;

public:
	/*
	 * Method to verify if actor is in target zone with deproject method
	 * @param _actor Actor
	 * @return true or false
	 */
	bool IsActorInTargetZone(const TObjectPtr<AActor>& _actor) const;

	/**
	 * Remove actor in detection list
	 * @param _actor Actor to remove
	 */
	void RemoveActorDetection(const TObjectPtr<AActor>& _actor);

private:
	UFUNCTION()
	void SetScoping(bool bCond);
	UFUNCTION()
	void OverheatIncrease(const float _percentageOverheat);

	UFUNCTION(BlueprintCallable)
	void SetColor(float _percent) const;

	UFUNCTION()
	void SetTextMarkedEnemy(int _marked, AActor* _actorMark);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void InitWithDelay() override;
	virtual void RegisterEvents() override;
	virtual void NativePreConstruct() override;

	void Resize(FViewport* ViewPort, uint32 val);

	void SetSizeOverlay(FVector2D _size, bool _needApplied);
	void ResizeFullScreen(bool bArg);
	FVector2D GetSizeFromPercent(const float _percent) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif WITH_EDITOR
};
