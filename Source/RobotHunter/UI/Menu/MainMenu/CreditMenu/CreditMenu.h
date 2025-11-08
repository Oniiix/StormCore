// 

#pragma once

#include "CoreMinimal.h"
#include "PeopleNamePrefab.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Template/Prefab/SeparatorPrefab/SeparatorPrefab.h"
#include "CreditMenu.generated.h"

USTRUCT()
struct FTypeSection
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText nameSection;

	UPROPERTY(EditAnywhere)
	TArray<FString> names;
};

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCreditMenu : public UBaseMenu
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRefresh);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnRefresh onBack;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> vbNames;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> backButton;

	UPROPERTY(EditAnywhere, Category = "Custom|Prefab")
	TSubclassOf<USpacer> spacerReference;

	UPROPERTY(EditAnywhere, Category = "Custom|Prefab")
	TSubclassOf<USeparatorPrefab> titleSectionReference;

	UPROPERTY(EditAnywhere, Category = "Custom|Prefab")
	TSubclassOf<UPeopleNamePrefab> namePrefabReference;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<FTypeSection> sections;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatName = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom", meta= (UIMin = 0, ClampMin = 0))
	float sizeSpacer = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Custom", meta = (AllowPrivateAccess))
	bool isInGame = false;

public:
	virtual void NativePreConstruct() override;

private:
	virtual void Init() override;
	virtual void Refresh() override;
	virtual void RegisterEvents() override;
	UFUNCTION(BlueprintCallable)
	void Back();
};
