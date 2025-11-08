#pragma once

#include "CoreMinimal.h"
#include "../InteractableActor.h"
#include "Components/SphereComponent.h"
#include "../../../CCC/Character/Components/Inventory/InventoryComponent.h"
#include "EndDayActor.generated.h"

class ARHWagonActor;

UCLASS()
class ROBOTHUNTER_API AEndDayActor : public AInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<USphereComponent> interactionSphere;

	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (UIMin = 10.0f, ClampMin = 10.0f))
	float interactionRadius;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float holdingTime;

	UPROPERTY(EditAnywhere, Category = "Level")
	TSoftObjectPtr<UWorld> hubLevel = nullptr;

	UPROPERTY()
	TArray<AMainCharacter*> players;

	//UPROPERTY()
	//TObjectPtr<ARHWagonActor> wagon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInventoryComponent> inventory = nullptr;

	bool inputsSetup;
	float holdTime;
	FTimerHandle holdTimer;

public:
	AEndDayActor();

private:

	void UpdateInteractionSphere();
	void UpdateHoldTime();
	void EndDay();

	void UpdatePlayerMoney(AMainCharacter* _player);

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	virtual void Interacted(AMainCharacter* _character) override;
	void UpdateInventoryFromPlayer(AMainCharacter* _player);
};
