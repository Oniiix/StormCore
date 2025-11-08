// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Game/Interactable/Loot/Carcass/CarcassActor.h"
#include "RobotHunter/CCC/Character/Components/Inventory/InventoryComponent.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/FireFLies/FirefliesActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/Interactable/Handcar/HandleHandcarActor.h"
#include "RobotHunter/Game/Interactable/Handcar/NitroHandcarActor.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/NailGun/NailGunActor.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"
#include "RobotHunter/Game/GPE/Projectile/FireFliesProjectile/FireFliesProjectile.h"
#include <RobotHunter/CCC/Character/Animations/CharaAnim.h>
#include "RobotHunter/Game/GPE/Members/CalfActors/SeismicKick/SBoxActor.h"
#include "RobotHunter/Game/Managers/Time/TimeManager.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "RobotHunter/Game/Interactable/Handcar/StopHandcarActor.h"
#include "RobotHunter/Game/Interactable/Hub/StockActor/StockInteractableActor.h"
#include "NetworkGPEComponent.generated.h"

#define NETWORK_GPE_COMPONENT_USE_LOG false

/// <summary>
/// Component for all GPE's networking
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UNetworkGPEComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNetworkGPEComponent();

	UFUNCTION(Server, Unreliable)
	void ServerRpc_DestroyCustomActor(ACustomActor* _actor);
	UFUNCTION(Server, Unreliable)
	void ServerRpc_AddToInventory(UInventoryComponent* _inventory,const FString& _name, const int _quantity);
	UFUNCTION(Server, Unreliable)
	void ServerRpc_DropFromInventory(UInventoryComponent* _inventory,const FString& _name, const int _quantity);
	UFUNCTION(Server, Unreliable)
	void ServerRpc_DropAllFromInventory(UInventoryComponent* _inventory, const FString& _name);
	UFUNCTION(Server, Unreliable)
	void ServerRpc_ClearInventory(UInventoryComponent* _inventory);		
	
	UFUNCTION(Server, Unreliable) void ServerRPC_SpawnNailgunProjectile(ANailGunActor* _nailgun, const FHitResult& _result, ANailgunProjectile* _spawnedNail);



#pragma region Handcar
	UFUNCTION(Server, Unreliable) void ServerRPC_AddCallingStop(ACustomHandcarActor* _handcar, AStopHandcarActor* _stop);
	UFUNCTION(NetMulticast, Unreliable) void MulticastRPC_AddCallingStop(ACustomHandcarActor* _handcar, AStopHandcarActor* _stop);
	UFUNCTION(Server, Unreliable) void ServerRPC_UpdateInAutoMode(ACustomHandcarActor* _handcar, const bool _inAutoMode);
	UFUNCTION(Server, Unreliable) void ServerRPC_SetInAutoMode(ACustomHandcarActor* _handcar, const bool _inAutoMode);
	UFUNCTION(NetMulticast, Unreliable) void MulticastRPC_SetInAutoMode(ACustomHandcarActor* _handcar, const bool _inAutoMode);
	UFUNCTION(Server, Unreliable) void ServerRPC_UpdateCalledHandcar(AStopHandcarActor* _stop, const bool _called);

#pragma region Brake
	UFUNCTION(Server, Unreliable) void ServerRpc_SetReleasedBrakeLever(ABrakeHandcarActor* _brakeHandcar, const bool _isReleased);
#pragma endregion
	
#pragma region Nitro
	UFUNCTION(Server, Unreliable) void ServerRpc_SetNitroPlayer(ANitroHandcarActor* _nitroHandcar, AMainCharacter* _player);

	UFUNCTION(Server, Unreliable) void ServerRpc_SetNitroAcceleration(ANitroHandcarActor* _nitroHandcar, const float _newUpdatedNitroAcceleration);
#pragma endregion

#pragma region Target
	UFUNCTION(Server, Unreliable) void ServerRPC_UpdateCurrentIndex(ARailsTargetActor* _target, const int _index, const bool _isDeparture);
#pragma endregion

#pragma endregion 
	
	UFUNCTION(Server, Unreliable) void ServerRpc_LaunchMovable(UMovableComponent* _comp, const FVector& _dir);

#pragma region Anim
	UFUNCTION(Server, Unreliable) void ServerRpc_SetIsScoping(UCharaAnim* _anim,bool _status);
#pragma endregion

#pragma region SeismicKick
	UFUNCTION(Server, Unreliable) void ServerRpc_SpawnStone(ASBoxActor* _boxActor);
#pragma endregion


	UFUNCTION(Server, Unreliable) void ServerRpc_CallOnInteractEvent(AInteractableActor* _interactActor,AMainCharacter* _character);
	UFUNCTION(NetMulticast, WithValidation, Unreliable) void ClientRpc_CallOnInteractEvent(AInteractableActor* _interactActor,AMainCharacter* _character);

#pragma region TimeManager
	UFUNCTION(Server, Unreliable) void ServerRPC_CurrentTime(ATimeManager* _timeManager);
	UFUNCTION(NetMulticast, Unreliable) void MulticastRPC_CurrentTime(ATimeManager* _timeManager, const float _currentMinute, const float _currentHour);
#pragma endregion

#pragma region stock
	UFUNCTION(Server,Reliable) void ServerRPC_SetCurrentCharacter(AStockInteractableActor* _stock,const FString& _currentCharacter);
	UFUNCTION(Server, Unreliable) void ServerRPC_SetPositionClient(AMainCharacter* _character, const FVector& _positionActor, const FRotator& _rotationActor);
	UFUNCTION(Server,Reliable) void ServerRPC_RequestSetInvisibleForTarget(AMainCharacter* Target, bool bInvisible);
	UFUNCTION(Server,Reliable) void SetReplicate(const bool _value);
	
#pragma endregion 
	
	UFUNCTION(Server, Unreliable) void ServerRpc_Action(AReceiver* _receiver);
};
