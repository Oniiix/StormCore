// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGPEComponent.h"

#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Game/GPE/Handcar/CustomHandcarActor.h"
#include "RobotHunter/Game/Interactable/Hub/StockActor/StockInteractableActor.h"


UNetworkGPEComponent::UNetworkGPEComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNetworkGPEComponent::SetReplicate_Implementation(const bool _value)
{
	ACustomGameState* _game = Cast<ACustomGameState>(GetWorld()->GetGameState());

	for (int i = 0; i < _game->GetPlayers().Num(); i++)
	{
		//_game->GetPlayers()[i]->SetReplicates(_value);
		LOG_COLOR(true, " SetReplicate_Implementation, Player = " + FString::FromInt(i), FColor::Cyan);
	}
}
void UNetworkGPEComponent::ServerRPC_SetPositionClient_Implementation(AMainCharacter* _character, const FVector& _positionActor, const FRotator& _rotationActor)
{
	LOG_COLOR(true, "ServerRPC_SetPositionClient", MAGENTA);
	_character->SetActorLocation(_positionActor + FVector(0, 0, 75));
	_character->SetMeshRotation(FRotator(_character->GetActorRotation().Pitch,_rotationActor.Yaw +20, _character->GetActorRotation().Roll),true);
}

void UNetworkGPEComponent::ServerRPC_SetCurrentCharacter_Implementation(AStockInteractableActor* _stock,const FString& _currentCharacter)
{
	_stock->SetCurrentCharacter(_currentCharacter);
}

void UNetworkGPEComponent::ServerRPC_RequestSetInvisibleForTarget_Implementation(AMainCharacter* Target, bool bInvisible)
{
	if (!Target) return;
	LOG_COLOR(true, "ServerRPC_RequestSetInvisibleForTarget", MAGENTA);
	
	if (USkeletalMeshComponent* Mesh = Target->FindComponentByClass<USkeletalMeshComponent>())
	{
		Mesh->SetOnlyOwnerSee(bInvisible); // Visible uniquement pour le propriétaire (Client/Serveur spécifique).
		for ( TObjectPtr<AMembersActor> _memberActor  : Target->GetUsingMembers())
		{
			if (!_memberActor)
				continue;
			for ( USkeletalMeshComponent* _memberMesh  : _memberActor->GetMemberMeshs())
			{
				_memberMesh->SetOnlyOwnerSee(bInvisible);
			}
		}
	}
}

void UNetworkGPEComponent::ServerRpc_ClearInventory_Implementation(UInventoryComponent* _inventory)
{
	if (!_inventory)
		return;
	_inventory->ClearInventory();
}

void UNetworkGPEComponent::ServerRpc_DropAllFromInventory_Implementation(UInventoryComponent* _inventory, const FString& _name)
{
	if (!_inventory)
		return;
	_inventory->DropAllItem(_name);
}

void UNetworkGPEComponent::ServerRpc_AddToInventory_Implementation(UInventoryComponent* _inventory, const FString& _name, const int _quantity)
{
	if (!_inventory)
		return;
	_inventory->AddItem(_name, _quantity);
}

void UNetworkGPEComponent::ServerRpc_DropFromInventory_Implementation(UInventoryComponent* _inventory, const FString& _name, const int _quantity)
{
	if (!_inventory)
		return;
	_inventory->DropItem(_name, _quantity);
}

void UNetworkGPEComponent::ServerRpc_DestroyCustomActor_Implementation(ACustomActor* _actor)
{
	if (!_actor)
		return;
	_actor->SetLifeSpan(0.1);
}

void UNetworkGPEComponent::ServerRPC_SpawnNailgunProjectile_Implementation(ANailGunActor* _nailgun, const FHitResult& _result, ANailgunProjectile* _spawnedNail)
{
	//_nailgun->OnShoot().Broadcast()
	//ANailgunProjectile* _projectile = GetWorld()->SpawnActor<ANailgunProjectile>(_nailgun->GetNailgunProjectilRef(), _result.ImpactPoint, _nailgun->GetMainCharacter()->GetMesh()->GetRelativeRotation());
	//_projectile->SetNailgun(_nailgun);
	//_projectile->AttachToActor(_result.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
	//_projectile->Init();
	//_spawnedNail = _projectile;
}




#pragma region Handcar
void UNetworkGPEComponent::ServerRPC_AddCallingStop_Implementation(ACustomHandcarActor* _handcar, AStopHandcarActor* _stop)
{
	if (_handcar)
		_handcar->AddCallingStop_Internal(_stop);
}

void UNetworkGPEComponent::MulticastRPC_AddCallingStop_Implementation(ACustomHandcarActor* _handcar, AStopHandcarActor* _stop)
{
	if (_handcar)
		_handcar->AddCallingStop_Internal(_stop);
}

void UNetworkGPEComponent::ServerRPC_UpdateInAutoMode_Implementation(ACustomHandcarActor* _handcar, const bool _inAutoMode)
{
	if (_handcar)
		_handcar->OnRep_UpdateInAutoMode(_inAutoMode);
}

void UNetworkGPEComponent::ServerRPC_SetInAutoMode_Implementation(ACustomHandcarActor* _handcar, const bool _inAutoMode)
{
	if (_handcar)
		_handcar->SetInAutoMode(_inAutoMode);
}

void UNetworkGPEComponent::MulticastRPC_SetInAutoMode_Implementation(ACustomHandcarActor* _handcar, const bool _inAutoMode)
{
	if (_handcar)
		_handcar->SetInAutoMode(_inAutoMode);
}

void UNetworkGPEComponent::ServerRPC_UpdateCalledHandcar_Implementation(AStopHandcarActor* _stop, const bool _called)
{
	if (_stop)
		_stop->OnRep_UpdateCalledHandcar(_called);
}

#pragma region Brake

void UNetworkGPEComponent::ServerRpc_SetReleasedBrakeLever_Implementation(ABrakeHandcarActor* _brakeHandcar, const bool _isReleased)
{
	if (!_brakeHandcar)
		return;

	if (_brakeHandcar->GetHandcar())
		_brakeHandcar->GetHandcar()->SetInAutoMode(false);

	_brakeHandcar->SetReleased(_isReleased);
	_brakeHandcar->InstantBrake(_isReleased);
	_brakeHandcar->GetOnReleaseBrake().Broadcast(_isReleased);
}

#pragma endregion

#pragma region Nitro

void UNetworkGPEComponent::ServerRpc_SetNitroPlayer_Implementation(ANitroHandcarActor* _nitroHandcar, AMainCharacter* _player)
{
	if (!_nitroHandcar) return;

	if (!_player)
	{
		_nitroHandcar->SetUpdatedNitroAcceleration(0);
		_nitroHandcar->GetNitroPlayer()->SetIsUsingNitroHandcar(false);
		_nitroHandcar->GetNitroPlayer()->SetIsUsingHandcar(false);
		_nitroHandcar->GetOnUseControls().Broadcast(false);

		_nitroHandcar->GetNitroPlayer()->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		_nitroHandcar->GetNitroPlayer()->AttachToComponent(_nitroHandcar->GetHandcar()->GetHandcarMesh(), FAttachmentTransformRules::KeepWorldTransform);
		_nitroHandcar->GetNitroPlayer()->SetMeshRotation(FRotator(0, 0, 0), true);
	}
	else
	{
		_player->SetIsUsingNitroHandcar(true);
		_nitroHandcar->GetOnUseControls().Broadcast(true);
		_player->AttachToComponent(_nitroHandcar->GetDriverSeat(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "DS_Socket");
		_player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}

	if (_nitroHandcar->GetHandcar())
	{
		ABrakeHandcarActor* _brake = _nitroHandcar->GetHandcar()->GetBrakeActor();
		if (_brake)
		{
			_brake->SetReleased(_player != nullptr);
			_brake->InstantBrake(_player != nullptr);
		}
	}

	_nitroHandcar->SetNitroPlayer(_player);
	//_nitroHandcar->GetOnUseControls().Broadcast(_player != nullptr);
}


void UNetworkGPEComponent::ServerRpc_SetNitroAcceleration_Implementation(ANitroHandcarActor* _nitroHandcar, const float _newUpdatedNitroAcceleration)
{
	if (!_nitroHandcar) return;
	_nitroHandcar->SetUpdatedNitroAcceleration(_newUpdatedNitroAcceleration);
}

#pragma endregion

#pragma region Target
void UNetworkGPEComponent::ServerRPC_UpdateCurrentIndex_Implementation(ARailsTargetActor* _target, const int _index, const bool _isDeparture)
{
	if (_target)
	{
		if (_isDeparture)
			_target->OnRep_UpdateIndexDeparture(_index);
		else
			_target->OnRep_UpdateIndexReturn(_index);
	}
}
#pragma endregion

#pragma endregion


void UNetworkGPEComponent::ServerRpc_LaunchMovable_Implementation(UMovableComponent* _comp, const FVector& _dir)
{
	if (_comp)
		_comp->Launch(_dir);
}

void UNetworkGPEComponent::ServerRpc_SetIsScoping_Implementation(UCharaAnim* _anim, bool _status)
{
	if (_anim)
	{
		//LOG(true, "CA MARCHE");
		_anim->SetIsScoping(_status);
	}
	//else
		//LOG(true, "CA MARCHE PAS");

}


void UNetworkGPEComponent::ServerRpc_SpawnStone_Implementation(ASBoxActor* _boxActor)
{
	if (_boxActor)
	{
		//LOG_COLOR(true, "Spawn edfsdfsf Stone", YELLOW);
		//_boxActor->SpawnStone();
	}
}

void UNetworkGPEComponent::ServerRpc_CallOnInteractEvent_Implementation(AInteractableActor* _interactActor, AMainCharacter* _character)
{
	if (_interactActor)
		_interactActor->OnInteract().Broadcast(_character);
}


bool UNetworkGPEComponent::ClientRpc_CallOnInteractEvent_Validate(AInteractableActor* _interactActor,AMainCharacter* _character)
{
	return _interactActor && !(_interactActor->HasAuthority());
}

void UNetworkGPEComponent::ClientRpc_CallOnInteractEvent_Implementation(AInteractableActor* _interactActor,AMainCharacter* _character)
{
	_interactActor->OnInteract().Broadcast(_character);
}

void UNetworkGPEComponent::ServerRPC_CurrentTime_Implementation(ATimeManager* _timeManager)
{
	if (!_timeManager)
	{
		LOG_ERROR(true, "TIMEMANAGER NULL");
		return;
	}
	MulticastRPC_CurrentTime(_timeManager, _timeManager->GetInGameMinutes(), _timeManager->GetInGameHour());
}

void UNetworkGPEComponent::MulticastRPC_CurrentTime_Implementation(ATimeManager* _timeManager, const float _currentMinute, const float _currentHour)
{
	_timeManager->SetMinute(_currentMinute);
	_timeManager->SetHour(_currentHour);
}

void UNetworkGPEComponent::ServerRpc_Action_Implementation(AReceiver* _receiver)
{
	if (_receiver)
		_receiver->Action();
}
