#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "WeakPointComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UWeakPointComponent : public UCapsuleComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "WeakPoint_Property", meta = (Umin = 0.f, ClampMin = 0.f))
	float damageMultiplier = 2.f;

public:
	FORCEINLINE void SetDamageMultiplier(const float _damageMultiplier) { damageMultiplier = _damageMultiplier; }
	FORCEINLINE float GetDamageMultiplier() const { return damageMultiplier; }
};
