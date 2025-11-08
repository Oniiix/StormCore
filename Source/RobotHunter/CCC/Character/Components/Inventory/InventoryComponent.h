#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <RobotHunter/Game/Datas/Drop/DropData.h>
#include <RobotHunter/Utils/CustomMap.h>
#include "InventoryComponent.generated.h"

class AEgg;
class ALootActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UInventoryComponent : public UActorComponent
{
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddItem, int, weight);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddEgg, int, weight);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveItem, int, weight);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveEgg, int, weight);
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Money")
	float money;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int maxWeightBiomass = 0;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TMap<FString,UDropData*> inventory;
	UPROPERTY(ReplicatedUsing= OnRep_Test, VisibleAnywhere, Category = "Inventory")
	FCustomMap itemsQuantity;

	UPROPERTY(VisibleAnywhere)
	int currentRessources;
	UPROPERTY(VisibleAnywhere)
	bool hasEgg = false;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AEgg> currentEgg = nullptr;

	FOnAddItem onAddItem;
	FOnAddEgg onAddEgg;
	FOnRemoveItem onRemoveItem;
	FOnRemoveEgg onRemoveEgg;
	
	int qt = 0;

	TObjectPtr<class AMainCharacter> mainCharacter = nullptr;
public:
	FORCEINLINE void SetMoney(const float _money) { money = _money; }
	FORCEINLINE float GetMoney() const { return money; }
	FORCEINLINE float GetMaxWeightBiomass() const { return maxWeightBiomass; }
	FORCEINLINE bool GetHasEgg() const { return hasEgg; }
	FORCEINLINE TObjectPtr<AEgg> GetCurrentEgg() const { return currentEgg; }

	FORCEINLINE TMap<FString, UDropData*> GetInventory() const { return inventory; }
	FORCEINLINE FCustomMap GetItemsQuantity() const { return itemsQuantity; }
	FORCEINLINE FOnAddItem GetOnAddItem() { return onAddItem; }
	FORCEINLINE FOnAddEgg GetOnAddEgg() { return onAddEgg; }
	FORCEINLINE FOnRemoveItem GetOnRemoveItem() { return onRemoveItem; }
	FORCEINLINE FOnRemoveEgg GetOnRemoveEgg() { return onRemoveEgg; }
	

public:
	FORCEINLINE void SetMainCharacter(AMainCharacter* _chara) { mainCharacter = _chara; }
	UInventoryComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float _deltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	void AddItem(const FString& _name, const int _qty = 1);
	void AddItemToInventory(const FString& _name, const int _qty = 1);
	void DropItem(const FString& _key, const int _qty = 1);
	void DropItemFromInventory(const FString& _key, const int _qty = 1);
	void DropAllItem(const FString& _key);
	void DropAllItemFromInventory(const FString& _key);

	void AddEgg(AEgg* _egg);
	void DropEgg();
	void ClearInventory();
	void ClearAllInventory();

	float GetTotalInventoryMoney();
	float GetTotalInventoryWeight();

	const int GetQuantityByKey(const FString& _key);
	const int GetAllDropsValueByKey(const FString& _key);
	const float GetWeightByQuantity(const FString& _key,const int& _qty);

#pragma region Network
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UFUNCTION() void OnRep_Test();
#pragma endregion
};
