#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Flower/V2/ZoneFlower/ZoneFlowerV2.h"
#include "DamageZoneFlowerV2.generated.h"

UCLASS(Abstract)
class ROBOTHUNTER_API ADamageZoneFlowerV2 : public AZoneFlowerV2
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Custom Property|Damage")
	float damage;

	UPROPERTY(EditAnywhere, Category = "Custom Property|Modifiers")
	float damageModifier;

	float initialDamage;

public:
	ADamageZoneFlowerV2();

private:
	virtual void ApplyDamageToActorsInArea();

protected:
	virtual void Init() override;
	virtual void ApplyEffectToActorsInArea() override;

	virtual void EnableModifiers() override;
	virtual void DisableModifiers() override;
};



