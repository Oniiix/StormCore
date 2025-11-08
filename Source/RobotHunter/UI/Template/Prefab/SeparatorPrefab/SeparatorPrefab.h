// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Spacer.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "SeparatorPrefab.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USeparatorPrefab final : public UPrefab
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USpacer> spacerWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> nameCategory;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString format = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText textDisplay = TEXTSTR("example");

public:
	virtual void NativePreConstruct() override;
	void SetFirst() const;
	void SetCategoryName(const FString& _name) const;
};
