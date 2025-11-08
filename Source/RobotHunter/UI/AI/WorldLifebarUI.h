#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "RobotHunter/UI/FloatingUI/FloatingUI.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "WorldLifebarUI.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UWorldLifebarUI final : public UFloatingUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHit onHit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UProgressBar> lifebar = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textLife = nullptr;

public:
	UFUNCTION()
	FORCEINLINE void CallbackOnHit() const { onHit.Broadcast(); }

	void UpdateLifeBar(const float _currentLife, const float _maxLife) const;
};
