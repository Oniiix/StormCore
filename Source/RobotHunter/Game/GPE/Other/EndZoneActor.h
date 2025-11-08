

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "EndZoneActor.generated.h"

UCLASS()
class ROBOTHUNTER_API AEndZoneActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> triggerBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom")
	ETypeChange travelChange = ETypeChange::IntroToHub;

	UPROPERTY(EditAnywhere, category = "Custom")
	TSoftObjectPtr<UWorld> travelMap = nullptr;

	UPROPERTY(EditAnywhere, category = "Custom")
	bool createServer = false;

	UCustomGameInstance* gi = nullptr;

public:	
	AEndZoneActor();

protected:
	void NotifyActorBeginOverlap(AActor* _otherActor) override;

private:
	UFUNCTION() void Travel();
#pragma region Succés Steam
	void UnlockAchievement(const FName& _achievementName);
#pragma endregion
};
