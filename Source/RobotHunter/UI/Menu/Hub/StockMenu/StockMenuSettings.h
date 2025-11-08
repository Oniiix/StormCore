#pragma once

#include "CoreMinimal.h"
#include "../../../BaseSettings.h"
#include "StockMenuSettings.generated.h"

class ALeftArmActor;
class ARightArmActor;
class ACalfActor;
class AThighActor;
class UButtonMemberPrefab;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UStockMenuSettings final : public UBaseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|Left Arm")
	TArray<TSubclassOf<ALeftArmActor>> leftArms;
	UPROPERTY(EditAnywhere, Category = "Custom|Right Arm")
	TArray<TSubclassOf<ARightArmActor>> rightArms;
	UPROPERTY(EditAnywhere, Category = "Custom|Calf")
	TArray<TSubclassOf<ACalfActor>> calfs;
	UPROPERTY(EditAnywhere, Category = "Custom|Thigh")
	TArray<TSubclassOf<AThighActor>> thighs;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UButtonMemberPrefab> templateButton;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString format = "<normal>{0}</>";

public:
	FORCEINLINE TArray<TSubclassOf<ALeftArmActor>> GetLeftArms() const { return leftArms; }
	FORCEINLINE TArray<TSubclassOf<ARightArmActor>> GetRightArms() const { return rightArms; }
	FORCEINLINE TArray<TSubclassOf<ACalfActor>> GetCalfs() const { return calfs; }
	FORCEINLINE TArray<TSubclassOf<AThighActor>> GetThighs() const { return thighs; }

	FORCEINLINE TSubclassOf<UButtonMemberPrefab> GetTemplateButton() const { return templateButton; }

	FORCEINLINE FString GetFormatText() const { return format; }
};
