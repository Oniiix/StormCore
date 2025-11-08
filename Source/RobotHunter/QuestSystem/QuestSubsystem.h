// 

#pragma once

#include "CoreMinimal.h"
#include "Asset/QuestAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "QuestSubsystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAdded, UQuestAsset*, _asset);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestRemoved, UQuestAsset*, _asset);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestValidate, const UQuestAsset*, _asset);


UCLASS()
class ROBOTHUNTER_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UQuestAsset*> quests;

	UPROPERTY()
	TArray<int> questsComplete;

	/**
	 * Event when a quest is added to list
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestSystem|SubSystem|Event", meta = (AllowPrivateAccess))
	FOnQuestAdded onQuestAdded;
	/**
	 * Event when a quest is removed to list
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestSystem|SubSystem|Event", meta = (AllowPrivateAccess))
	FOnQuestRemoved onQuestRemoved;
	/**
	 * Event when a quest is validated
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestSystem|SubSystem|Event", meta = (AllowPrivateAccess))
	FOnQuestValidate onQuestValidate;

public:
	/**
	 * Add quest to quests list
	 * @param _asset DataAsset of quest
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	void AddQuest(UQuestAsset* _asset);
	/**
	 * Remove quest to quests list
	 * @param _asset DataAsset of quest
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	void RemoveQuest(UQuestAsset* _asset);
	/**
	 * Check if quest exist in list
	 * @param _asset DataAsset of quest
	 * @return True or False
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	bool ExistQuest(const UQuestAsset* _asset) const;
	/**
	 * Get quest from an index
	 * @param _index Index
	 * @return QuestData pointer or null
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	UQuestAsset* GetQuestFromIndex(const int _index);
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	void ValidQuest(UQuestAsset* _asset);

	/**
	 * Get all quests in list
	 * @return An array of quests
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	TArray<UQuestAsset*> GetQuests() const { return quests; }

	/**
	 * Clear all quests
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	void ClearAllQuests();

	/**
	 * Clear all quests without UI manage
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	void ForceClearAllQuests();

	/**
	 * Return the list of quest complete
	 * @return List of id of quest completed
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	TArray<int> GetQuestCompleted() const { return questsComplete; }

	/**
	 * Return the list of quest complete
	 * @return List of quest completed
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	bool IsCompleteQuest(UQuestAsset* _quest) const
	{
		return questsComplete.Contains(_quest->GetIdQuest());
	}

	/**
	 * Set list of complete quest	
	 */
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Subsystem")
	void SetQuestCompleteList(TArray<int> _quests) { questsComplete.Append(_quests); }

	FORCEINLINE FOnQuestAdded& OnQuestAdded() { return onQuestAdded; }
	FORCEINLINE FOnQuestRemoved& OnQuestRemoved() { return onQuestRemoved; }
	FORCEINLINE FOnQuestValidate& OnValidateQuest() { return onQuestValidate; }
};
