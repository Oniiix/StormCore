// 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <Components/WidgetComponent.h>
#include "../Actor/QuestWidget.h"
#include "QuestAsset.generated.h"

class UQuestDropdownTemplate;

UENUM()
enum EQuestType { Primary, Secondary };

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UQuestAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom", meta = (AllowPrivateAccess))
	int Id;

	UPROPERTY(VisibleAnywhere, Category = "Custom")
	bool isValidate = false;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText text = TEXTSTR("Text of quest");

	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<UQuestAsset*> conditionQuests;

	/** The list of arguments to pass in text. They will be replace {} */
	UPROPERTY(EditAnywhere, Category = "Custom")
	TArray<int> Args;

	UPROPERTY(VisibleAnywhere, Category = "Custom|Debug")
	FString textShowed = "";

	UPROPERTY()
	UQuestDropdownTemplate* Widget;

	UPROPERTY(EditAnywhere)
	bool useWidget = false;
	UPROPERTY(EditAnywhere)
	bool searchByTag = false;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "useWidget && !searchByTag", EditConditionHides))
	TSubclassOf<AActor> questActor;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "useWidget && searchByTag", EditConditionHides))
	FName questActorTag;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "useWidget", EditConditionHides))
	FVector questLocationOffset;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "useWidget", EditConditionHides))
	TSubclassOf<AQuestWidget> questWidgetRef;
	UPROPERTY(EditAnywhere)
	TArray<AQuestWidget*> currentWidgets;


#if WITH_EDITOR

private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category="Unique")
	void AssignRandomUniqueValue();
#endif
#endif

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TSubclassOf<AActor> GetQuestActor() const { return questActor; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetQuestActorTag() const { return questActorTag; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetQuestLocationOffset() const { return questLocationOffset; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TSubclassOf<AQuestWidget> GetQuestWidgetRef() const { return questWidgetRef; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<AQuestWidget*>& GetCurrentQuestWidgets() { return currentWidgets; }

	FORCEINLINE bool GetIsValidate() const { return isValidate; }
	FORCEINLINE bool GetUseWidget() const { return useWidget; }
	FORCEINLINE bool GetSearchByTag() const { return searchByTag; }
	void SetCurrentQuestWidgets(AQuestWidget* _currentWidget) { currentWidgets.Add(_currentWidget); }
	FORCEINLINE int GetIdQuest() const { return Id; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<int> GetArgs() const { return Args; }


	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Data")
	FString GetTextQuest();
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Data")
	void SetArgsParam(int argNumber, const FText _parameter);

	void SetWidget(UQuestDropdownTemplate* _widget);
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Data")
	void UpdateParms(int _value);
	UFUNCTION(BlueprintCallable, Category = "QuestSystem|Data")
	void ResetParams();

	void ValidQuest();
	int CompleteConditionQuests();
};
