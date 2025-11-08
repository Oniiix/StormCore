#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "PowerLineV2.generated.h"

class AStormFlowerV2;

UCLASS()
class ROBOTHUNTER_API APowerLineV2 : public ACustomActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLightningActivation);

	UPROPERTY(EditAnywhere, Category = "Custom Property|Activation")
	bool useRandomActivationTime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Activation", meta = (EditCondition = "!useRandomActivationTime", EditConditionHides, UIMin = 0.1f, ClampMin = 0.1f))
	float activationTime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Activation", meta = (EditCondition = "useRandomActivationTime", EditConditionHides, UIMin = 0.1f, ClampMin = 0.1f))
	float minActivationTime;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Activation", meta = (EditCondition = "useRandomActivationTime", EditConditionHides, UIMin = 0.1f, ClampMin = 0.1f))
	float maxActivationTime;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector lineStart;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector lineEnd;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLightningActivation onLightningActivation;
	
	UPROPERTY()
	TObjectPtr<AStormFlowerV2> firstFlower;

	UPROPERTY()
	TObjectPtr<AStormFlowerV2> secondFlower;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	bool drawDebugPowerLine;

	FTimerHandle lightningTimer;

public:
	FORCEINLINE AStormFlowerV2* GetLinkedFlower(AStormFlowerV2* _baseFlower)
	{
		return firstFlower == _baseFlower ? secondFlower : firstFlower;
	}

public:
	APowerLineV2();

private:
	void LightningCooldown();

protected:
	virtual void DrawDebug() override;

public:
	void Init(const FVector& _lineStart, const FVector& _lineEnd, AStormFlowerV2* _firstFlower, AStormFlowerV2* _secondFlower);
	void ActivateLightning();
	void DestroyPowerLine();
};
