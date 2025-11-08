#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "KillerZone.generated.h"

class UGameSectionSettings;
class UCustomGameInstance;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API AKillerZone : public ACustomActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom|Trigger", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;
	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UGameSectionSettings> settings;

	UPROPERTY()
	TObjectPtr<UCustomGameInstance> GameInstance;

	// Coordonnées de téléportation
public:
	AKillerZone();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
