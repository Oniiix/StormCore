#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "CustomTransition.generated.h"

class UCustomState;
class UCustomFSM;

UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API UCustomTransition : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<UCustomState> nextState;

	UPROPERTY()
	TObjectPtr<UCustomFSM> fsm;

public:
	FORCEINLINE void SetFSM(UCustomFSM* _fsm) { fsm = _fsm; }

	FORCEINLINE TSubclassOf<UCustomState> GetNextState() const { return nextState; }

public:
	UCustomTransition();

public:
	virtual bool IsValidTransition();
};
