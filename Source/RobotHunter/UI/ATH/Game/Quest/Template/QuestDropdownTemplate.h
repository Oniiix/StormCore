// 

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/QuestSystem/Asset/QuestAsset.h"
#include "RobotHunter/UI/ATH/Game/Quest/QuestATH.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "QuestDropdownTemplate.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UQuestDropdownTemplate : public UPrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitQuest);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideQuest);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnValidate);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnInitQuest onInitQuest;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHideQuest onHideAnimation;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnValidate onValidate;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textTitle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	FString text;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta=(AllowPrivateAccess))
	TObjectPtr<UQuestAsset> questAsset;

	UPROPERTY()
	TObjectPtr<UVerticalBox> vbQuest;

	UPROPERTY()
	int indexVB;

	UPROPERTY()
	FString formatText;

	UPROPERTY()
	UQuestATH* ath;

public:
	FORCEINLINE TObjectPtr<UQuestAsset> GetQuest() const { return questAsset; }
	FORCEINLINE TObjectPtr<UTextPrefab> GetText() const { return textTitle; }
	FORCEINLINE const FString& GetFormatText() const { return formatText; }

	virtual void NativeConstruct() override;

	void InitializeQuest(UQuestAsset* _quest, const FString& _name, const FString& _formatQuestText,
	                     const TObjectPtr<UVerticalBox>& _questVB, int _indexVB, UQuestATH* _ath);
	void PlayHideAnimation() const;

	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Method")
	void RemoveWidgetFromList() const;

	void PlayValidAnimation() const;
};
