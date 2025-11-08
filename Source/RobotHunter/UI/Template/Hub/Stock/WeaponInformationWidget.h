#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "WeaponInformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UWeaponInformationWidget final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Blueprintable, meta = (BindWidget))
	TObjectPtr<UTextPrefab> textTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> video;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> description;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString nameFormat = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString descriptionFormat = "<normal>{0}</>";

public:
	void InitWidget(const FText& _title, const TObjectPtr<UMaterialInterface>& _player,
	                const FText& _description) const;
};
