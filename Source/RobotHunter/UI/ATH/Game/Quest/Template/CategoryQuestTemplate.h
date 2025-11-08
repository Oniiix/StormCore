// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/QuestSystem/Asset/QuestAsset.h"
#include "RobotHunter/UI/ATH/Game/Quest/QuestATH.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "CategoryQuestTemplate.generated.h"

class UQuestAsset;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCategoryQuestTemplate : public UPrefab
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitQuest, UQuestATH*, parentClass);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnInitQuest onInitQuest;

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Custom|BlueprintOnly", meta = (AllowPrivateAccess))
	const UQuestATH* questATH;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|BlueprintOnly", meta = (AllowPrivateAccess))
	const UQuestAsset* QuestAsset;

	UPROPERTY(BlueprintReadOnly, Category = "Custom|BlueprintOnly", meta = (AllowPrivateAccess))
	UVerticalBox* VB;

public:
	void InitializeCategory(UVerticalBox* _VB,
	                        const UQuestAsset* _QuestAsset, UQuestATH* QuestAth);
};
