// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/UI.h"
#include "FloatingUI.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class ROBOTHUNTER_API UFloatingUI : public UUI
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Custom")
	FText formatText = TEXTSTR("<italic>{0}</>");

public:
	virtual void Setup();
};
