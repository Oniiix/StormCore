#include "InventoryComponent.h"
#include "../../../../Game/Interactable/Loot/LootActor.h"
#include "../../../../Game/GPE/Nest/Egg.h"
#include <Net/UnrealNetwork.h>
#include <RobotHunter/Network/Component/NetworkGPEComponent.h>
#include <RobotHunter/Utils/DebugUtils.h>

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "Engine/ActorChannel.h"
#include "RobotHunter/Game/CustomGameState.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	money = 0.0f;
	currentRessources = 0;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	maxWeightBiomass = inventory["Biomass"]->GetMaxWeight();
}

void UInventoryComponent::TickComponent(float _deltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(_deltaTime, TickType, ThisTickFunction);
	if (!mainCharacter)
		return;
	FString _authorityText = mainCharacter->GetIsOwner() ? "[Owner] => " : "[Non Owner] => ";
	const TArray<FCustomKeyValuePair> _array = itemsQuantity.Get();
	const int _size=  _array.Num();
	for (int i = 0; i < _size; i++)
	{
		LOG_COLOR_TIME(true, _authorityText + " struct value -> Key => " + _array[i].key + " | Value => " + FString::FromInt(_array[i].value), FColor::Cyan, _deltaTime);
	}
	
}

void UInventoryComponent::AddItem(const FString& _name, const int _qty)
{
	if (!itemsQuantity.ContainsKey(_name))
		itemsQuantity.Add(_name, _qty);
	else
	{
		if ((_qty + itemsQuantity.GetValue(_name)) * inventory[_name]->GetWeight() > inventory[_name]->GetMaxWeight())
			itemsQuantity.UpdateValue(_name, (inventory[_name]->GetMaxWeight() / inventory[_name]->GetWeight()) - itemsQuantity.GetValue(_name));
		else
			itemsQuantity.UpdateValue(_name, _qty);
	}
	onAddItem.Broadcast(inventory[_name]->GetWeight() * _qty);
}

void UInventoryComponent::AddItemToInventory(const FString& _name, const int _qty)
{
	if (GetOwner()->HasAuthority())
		AddItem(_name, _qty);
	else
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_AddToInventory(this, _name, _qty);
}

void UInventoryComponent::DropItem(const FString& _name, const int _qty)
{
	if (!itemsQuantity.ContainsKey(_name))
		return;
	if (itemsQuantity.GetValue(_name) > _qty)
	{
		onRemoveItem.Broadcast(inventory[_name]->GetWeight() * _qty);
		itemsQuantity.UpdateValue(_name,-_qty);
	}
	else
		DropAllItemFromInventory(_name);
}

void UInventoryComponent::DropItemFromInventory(const FString& _key, const int _qty)
{
	if (GetOwner()->HasAuthority())
		DropItem(_key,_qty);
	else
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_DropFromInventory(this, _key, _qty);
}

void UInventoryComponent::DropAllItem(const FString& _name)
{
	if (!itemsQuantity.ContainsKey(_name))
		return;
	onRemoveItem.Broadcast(inventory[_name]->GetWeight() * itemsQuantity.GetValue(_name));
	itemsQuantity.SetValue(_name, 0);
}

void UInventoryComponent::DropAllItemFromInventory(const FString& _key)
{
	if (GetOwner()->HasAuthority())
		DropAllItem(_key); 
	else
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_DropAllFromInventory(this, _key);
}

void UInventoryComponent::AddEgg(AEgg* _egg)
{
	currentEgg = _egg;
	hasEgg = true;
	onAddEgg.Broadcast(_egg->GetWeight());
}

void UInventoryComponent::DropEgg()
{
	if (currentEgg)
		onRemoveEgg.Broadcast(currentEgg->GetWeight());

	currentEgg = nullptr;
	hasEgg = false;
}

void UInventoryComponent::ClearInventory()
{
	itemsQuantity.Empty();
}

void UInventoryComponent::ClearAllInventory()
{
	if (GetOwner()->HasAuthority())
		ClearInventory();
	else
		GetWorld()->GetFirstPlayerController()->GetComponentByClass<UNetworkGPEComponent>()->ServerRpc_ClearInventory(this);
}

float UInventoryComponent::GetTotalInventoryMoney()
{
	float _result = 0;
	const TArray<FCustomKeyValuePair>& _pairs = itemsQuantity.Get();
	for (FCustomKeyValuePair _items : _pairs)
		_result += (itemsQuantity.GetValue(_items.GetKey()) * inventory[_items.GetKey()]->GetPrice());
	return _result;
}

float UInventoryComponent::GetTotalInventoryWeight()
{
	float _result = 0;
	const TArray<FCustomKeyValuePair>& _pairs = itemsQuantity.Get();
	for (FCustomKeyValuePair _items : _pairs)
		_result += (itemsQuantity.GetValue(_items.GetKey()) * inventory[_items.GetKey()]->GetWeight());
	if (hasEgg)
		_result += currentEgg->GetWeight();
	return _result;
}

const int UInventoryComponent::GetQuantityByKey(const FString& _key)
{
	return itemsQuantity.ContainsKey(_key) ? itemsQuantity.GetValue(_key) : 0;
}

const int UInventoryComponent::GetAllDropsValueByKey(const FString& _key)
{
	return itemsQuantity.ContainsKey(_key) ? inventory[_key]->GetPrice() * itemsQuantity.GetValue(_key) : 0;
}

const float UInventoryComponent::GetWeightByQuantity(const FString& _key, const int& _qty)
{
	//if (itemsQuantity->ContainsKey(_key))
	//	return inventory[_key]->GetWeight() * _qty;
	//else
	//	return 0;

	return inventory.Contains(_key) ? inventory[_key]->GetWeight() * _qty : 0;
}

#pragma region Network
void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, itemsQuantity); 
}

void UInventoryComponent::OnRep_Test()
{
	LOG_COLOR(true, "INVENTORY: " + FString::FromInt(itemsQuantity.Num()), PINK);
}
#pragma endregion
