#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotHunter/Game/Interactable/Handcar/StopHandcarActor.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "StopHandcarManager.generated.h"

UCLASS()
class ROBOTHUNTER_API UStopHandcarManager : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	TMap<FString, AStopHandcarActor*> stops;

	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;

public:
	FORCEINLINE void SetHandcar(ACustomHandcarActor* _handcar) { handcar = _handcar; }
	FORCEINLINE TMap<FString, AStopHandcarActor*> GetStops() const { return stops; }

public:
	UStopHandcarManager();

public:
	void AddStop(AStopHandcarActor* _stop);
	void CallHandcar(AStopHandcarActor* _stop);
};
