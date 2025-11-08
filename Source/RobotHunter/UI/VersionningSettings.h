#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "VersionningSettings.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UVersionningSettings final : public UDataAsset
{
	GENERATED_BODY()

	/** The major version of game */
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (ClampMin = 0, UIMin = 0))
	int major = 0;
	/** The minor version of game */
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (ClampMin = 0, UIMin = 0))
	int minor = 0;

public:
	/**
	 * Get the major version in string formatted
	 * @return FString
	 */
	FORCEINLINE FString GetMajor() const { return INTSTR(major); }
	/**
	 * Get the minor version in string formatted
	 * @return FString
	 */
	FORCEINLINE FString GetMinor() const { return INTSTR(minor); }
};
