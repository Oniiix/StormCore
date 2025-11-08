// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSessionSettings.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UPlayerSessionSettings final : public UDataAsset
{
	GENERATED_BODY()

	/**
	 * Interval of time refresh ping
	 */
	UPROPERTY(EditAnywhere, Category = "Custom", meta= (Units ="seconds"))
	float timeToRefresh = 1.f;

public:
	/**
	 * Get time interval of refresh ping
	 * @return Time in seconds
	 */
	FORCEINLINE float GetTimeRefresh() const { return this->timeToRefresh; }
};
