// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameSectionSettings.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UGameSectionSettings final : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom")
	TSoftObjectPtr<UWorld> levelToHub = nullptr;

public:
	FORCEINLINE FString GetLevelToHub() const { return levelToHub.GetAssetName(); }
};
