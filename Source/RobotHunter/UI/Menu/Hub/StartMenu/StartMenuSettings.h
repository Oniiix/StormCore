#pragma once

#include "CoreMinimal.h"
#include "../../../BaseSettings.h"
#include "StartMenuSettings.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UStartMenuSettings final : public UBaseSettings
{
	GENERATED_BODY()

	/* The level to travel when clicked on play */
	UPROPERTY(EditAnywhere, Category = "Custom")
	TSoftObjectPtr<UWorld> levelToTravel = nullptr;

	//Level of test for GD
	UPROPERTY(EditAnywhere, Category = "Custom", AdvancedDisplay)
	TSoftObjectPtr<UWorld> sector3;

public:
	/**
	 * Return name of level to travel
	 * @return FString
	 */
	FString GetLevelToTravel() const;

	/**
	 * Get the name of map test
	 * @return Name of level asset
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	FString GetSector3Name() const;
};
