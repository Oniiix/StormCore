#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseSettings.generated.h"

class AMainHUD;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UBaseSettings : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<AMainHUD> hud;

public:
	/**
	 * Setup the settings with hud
	 * @param hudRef AMainHUD*
	 */
	virtual void Setup(const TObjectPtr<AMainHUD>& hudRef);
};
