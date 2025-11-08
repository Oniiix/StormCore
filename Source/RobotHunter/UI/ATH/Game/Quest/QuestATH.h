// 

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/Game/Managers/Quest/QuestManager.h"
#include "RobotHunter/QuestSystem/Asset/QuestAsset.h"
#include "RobotHunter/UI/UI.h"
#include "QuestATH.generated.h"

class UQuestDropdownTemplate;
class UCategoryQuestTemplate;

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UQuestATH : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> QuestVB;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UCategoryQuestTemplate> questCategoryTemplate;
	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UQuestDropdownTemplate> questDropDownTemplate;

	UPROPERTY(EditAnywhere, Category = "Custom|Category|Main")
	FString MainCategoryNameFormat = "";

	UPROPERTY(EditAnywhere, Category = "Custom|Category")
	FString questFormatText = "<normal>{0}</>";

	UPROPERTY(EditAnywhere, Category = "Custom|Category|Main")
	FText MainCategoryName = TEXTSTR("");

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<AQuestManager> questsManager;

	UPROPERTY(VisibleAnywhere, Category = "Custom")
	TMap<TObjectPtr<UQuestAsset>, int> slots;

protected:
	virtual void RegisterEvents() override;

private:
	UFUNCTION()
	void ValidateQuest(const UQuestAsset* QuestAsset);
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void AddQuest(UQuestAsset* QuestAsset);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CreateQuestWidget(UVerticalBox* _VB, UQuestAsset* _questAsset);

	virtual void Init() override;
	virtual void InitWithDelay() override;
	void RefreshQuest();
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void RecalculateSlot();
	int GetPositionSlotOfQuest(const UQuestAsset* Asset);

public:
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void RemoveQuest(UQuestAsset* QuestAsset);
};
