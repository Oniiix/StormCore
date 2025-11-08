

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KeyImageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UKeyImageDataAsset : public UDataAsset
{
	GENERATED_BODY()
	

	UPROPERTY(EditAnywhere)
	TMap<FKey, UMaterialInterface*> keyMap;

public:
	FORCEINLINE TMap<FKey, UMaterialInterface*> GetkeyMap() const { return keyMap; }
};
