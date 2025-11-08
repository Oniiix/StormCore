// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "HintInformation.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UHintInformation final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> title;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> description;

public:
	void SetHintInformation(const FText& _hintTitle, const FText& _hintSubtitle) const;
};
