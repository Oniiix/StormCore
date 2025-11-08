// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/ATH/TutorialATH.h"
#include "NewDraisineATH.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UNewDraisineATH : public UUI
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTutorialATH> tutoATH;

public:
	FORCEINLINE TObjectPtr<UTutorialATH> GetTutorialATH() const { return tutoATH; }
};
