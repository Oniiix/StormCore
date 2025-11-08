// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "ButtonPopupPrefab.generated.h"

class UPopupConfirmation;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UButtonPopupPrefab final : public UButtonPrefab
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UPopupConfirmation> parent;

public:
	UFUNCTION()
	void SetClickedButtonPopup();
	void Setup(UPopupConfirmation* PopupConfirmation, const FString& Key);
};
