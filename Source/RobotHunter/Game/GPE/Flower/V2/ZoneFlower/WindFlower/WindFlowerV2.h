#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/ZoneFlowerV2.h"
#include "WindFlowerV2.generated.h"

UCLASS()
class ROBOTHUNTER_API AWindFlowerV2 : public AZoneFlowerV2
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "CustomProperty|Force", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float forwardForce;

	UPROPERTY(EditAnywhere, Category = "CustomProperty|Force", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float upwardForce;

	UPROPERTY(EditAnywhere, Category = "CustomProperty|Modifiers", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float forwardForceModifier;

	UPROPERTY(EditAnywhere, Category = "CustomProperty|Modifiers", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float upwardForceModifier;

	bool useModifier;

public:
	AWindFlowerV2();

protected:
	virtual void StartActivePhase() override;

	virtual void ApplyEffectToActorsInArea() override;

	virtual void EnableModifiers() override;
	virtual void DisableModifiers() override;
};
