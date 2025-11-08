#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/Tuto/UI/DataAsset/TutorialHintAsset.h"
#include "UnlockZoneActor.generated.h"


UENUM()
enum class EUnlockType
{
	Unlock_Jump UMETA(DisplayName = "Jump"),
	Unlock_PrimaryWeapon UMETA(DisplayName = "PrimaryWeapon"),
	Unlock_SecondaryWeapon UMETA(DisplayName = "SecondaryWeapon"),
	Unlock_Kick UMETA(DisplayName = "Kick"),
	Unlock_MovementMember UMETA(DisplayName = "MovementMember"),
	Unlock_Scope UMETA(DisplayName = "Scope")
};

#define UNLOCK_ZONE_USE_LOG false

UCLASS()
class ROBOTHUNTER_API AUnlockZoneActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> hitBox = nullptr;

	UPROPERTY(EditAnywhere, Category="Custom")
	TArray<EUnlockType> unlockList;

	UPROPERTY(EditAnywhere, Category="Custom")
	TObjectPtr<UTutorialHintAsset> tutorialAsset = nullptr;

	UPROPERTY(EditAnywhere, Category="Custom")
	bool needTuto = false;

	TArray<TFunction<void(const TObjectPtr<AMainCharacter>&)>> functionList;

public:
	AUnlockZoneActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHitBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void CheckUnlockFunction(const TArray<EUnlockType>& _unlockList);
};
