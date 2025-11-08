#pragma once
#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Transmitter/TransmitterActor.h"
#include "RobotHunter/Game/Managers/Time/TimeManager.h"
#include "ClockActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AClockActor : public ATransmitterActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Instanced, Category="Custom|Clock")
	TObjectPtr<ATimeManager> timeManager = nullptr;

	UPROPERTY(EditAnywhere, Category="Custom|Clock")
	TArray<FIntVector2> hoursToActivate;

protected:
	virtual void BeginPlay()override;

private:
	UFUNCTION() void CheckTime(const int _hour, const int _minute);
	UFUNCTION() void CheckHour(const int _hour);

	void ClockActivate();

	UFUNCTION(CallInEditor, Category = "Action") void SortHoursToActivate();

	bool IsHourInferior(const FIntVector2& _a, const FIntVector2& _b);
};
