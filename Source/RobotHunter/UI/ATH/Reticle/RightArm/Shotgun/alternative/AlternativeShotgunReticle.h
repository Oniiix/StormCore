// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Overlay.h"
#include "RobotHunter/UI/ATH/Reticle/Reticle.h"
#include "AlternativeShotgunReticle.generated.h"

class ACadenceShotgunActor;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UAlternativeShotgunReticle final : public UReticle
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitWithDelay);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), category = "Custom")
	TObjectPtr<ACadenceShotgunActor> shotgun;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UOverlay> rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	float currentAngle = -36.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	float startShootAngle = 108.0f;

	/**
	 * Degrees in second
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	float rotationSpeed = 225.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	float EndCycleAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	bool on = true;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> AnimFail;

	UPROPERTY(Transient, BlueprintReadWrite, meta=(BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> AnimSuccess;

public:
	UFUNCTION(BlueprintCallable, Category = "Custom")
	virtual void InitWithDelay() override;

private:
	UFUNCTION()
	void ResetCycleFailed();
	UFUNCTION()
	void ResetCycleSuccess(const FVector& Vector, const FVector& Vector1);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void RegisterEvents() override;

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void ResetCycle(bool success);
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void UpdateAngleValue(const float InDeltaTime);
};
