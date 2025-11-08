// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InteractableActor.h"
#include <RobotHunter/CCC/Character/Components/Inventory/InventoryComponent.h>
#include "BagActor.generated.h"

#define BAG_USE_LOG false


UCLASS()
class ROBOTHUNTER_API ABagActor : public AInteractableActor
{
	GENERATED_BODY()
	
private:
#pragma region Inventory
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> inventoryComponent;
#pragma endregion Inventory

	bool isOnPlayer = true;
	float weight = 0;


public:
	FORCEINLINE void SetMainCharacter(class AMainCharacter* _chara) { inventoryComponent->SetMainCharacter(_chara); }
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return inventoryComponent; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return mesh; }
	FORCEINLINE bool GetIsOnPlayer() const { return isOnPlayer; }
	FORCEINLINE void SetIsOnPlayer(bool _status) { isOnPlayer = _status; }
	ABagActor();
	virtual void BeginPlay() override;
	virtual void Interacted(AMainCharacter* _character) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
