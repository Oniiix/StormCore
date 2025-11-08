

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <RobotHunter/QuestSystem/Asset/QuestAsset.h>
#include "QuestManager.generated.h"

UCLASS()
class ROBOTHUNTER_API AQuestManager : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAdded, UQuestAsset*, _asset);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestRemoved, UQuestAsset*, _asset);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestValidate, const UQuestAsset*, _asset);
public:
	/**
	 * Event when a quest is added to list
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestSystem|Event", meta = (AllowPrivateAccess))
	FOnQuestAdded onQuestAdded;
	/**
	 * Event when a quest is removed to list
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestSystem|Event", meta = (AllowPrivateAccess))
	FOnQuestRemoved onQuestRemoved;
	/**
	 * Event when a quest is validated
	 */
	UPROPERTY(BlueprintAssignable, Category = "QuestSystem|Event", meta = (AllowPrivateAccess))
	FOnQuestValidate onQuestValidate;
private:
	UPROPERTY(EditAnywhere)
	TMap<int, UQuestAsset*> quests;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<int> completeQuests;
	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UQuestAsset> addingQuest = nullptr;
public:	
	// Sets default values for this actor's properties
	AQuestManager();

	FORCEINLINE void SetQuestCompleteList(TArray<int> _quests) { completeQuests.Append(_quests); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitQuests();

	bool CheckTuto();

	void SetupQuests();

	void LoadQuests();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) void AddQuest(UQuestAsset* _asset);

	void InitQuestWidget(UQuestAsset* _asset);

	UFUNCTION(BlueprintCallable) bool ValidQuest(UQuestAsset* _asset);

	void ValidQuest_Internal(UQuestAsset* _asset);

	UFUNCTION(BlueprintCallable) bool CheckCompleteQuest(int _questId);

	UFUNCTION(BlueprintPure) TArray<UQuestAsset*> GetOnGoingQuests();

private:
	UFUNCTION(CallInEditor, Category = "QuestManager") void AddToQuest();
};
