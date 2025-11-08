// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/UI/Menu/Pause/Game/BonusInformation.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "PrefabRowBonus.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UPrefabRowBonus : public UPrefab
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> imageBonus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> countBonus;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> TextBonus;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> DescriptionBonus;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString FormatNumber = "<normal>x{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString TitleFormat = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString DescriptionFormat = "<normal>x{0}</>";

public:
	void SetBonus(FBonusInformation _information) const;
};
