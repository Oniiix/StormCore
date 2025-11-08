#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "TextDistanceWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UTextDistanceWidget final : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textDistance;

	//Placeholder of text to show distance. {0} is a variable given (example: 10 m)
	UPROPERTY(EditAnywhere, Category = "Custom")
	FString textDistancePlaceholder = "<normal>{0} m</>";

public:
	void SetDistance(const int& _distance) const;
};
