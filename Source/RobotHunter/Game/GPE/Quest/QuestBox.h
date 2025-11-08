

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include <RobotHunter/Game/Managers/Quest/QuestManager.h>
#include "QuestBox.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AQuestBox : public ATriggerBox
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<AQuestManager> questManager;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UQuestAsset> questToValid;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UQuestAsset> questToInit;


	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	
};
