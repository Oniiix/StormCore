// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkAIComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/AIsRefDataAsset.h"
#include "RobotHunter/Game/GPE/Nest/SpawnerAI.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/Arena/ArenaManager.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "RobotHunter/AI/Attack/Projectile/ParabolProjectile/ParaboleProjectile.h"
#include "RobotHunter/AI/Attack/Projectile/LineProjectile/LineProjectile.h"
#include "RobotHunter/AI/Component/Camouflage/CamouflageComponent.h"

// Sets default values for this component's properties
UNetworkAIComponent::UNetworkAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma region AI-RPC
void UNetworkAIComponent::ServerRPC_AIBehaviour_Implementation(AAI_Base* _AI, const int _token)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC AI behaviour", MAGENTA);
	//if (!_AI)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC AI behaviour - Failed");
	//	return;
	//}
	//_AI->EnableBehaviour(_token);
}

void UNetworkAIComponent::ServerRPC_LoadManagement_Implementation(AArena* _arena, const bool _isLoaded)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC load management", MAGENTA);
	//if (!_arena)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC load management - Failed");
	//	return;
	//}
	//_arena->LoadManagement(_isLoaded);
}

void UNetworkAIComponent::ServerRPC_EvolutionManagement_Implementation(AArena* _arena, const int _currentEvolutionState)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC evolution management", MAGENTA);
	//if (!_arena)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC evolution management - Failed");
	//	return;
	//}
	//_arena->SetCurrentEvolutionState(_currentEvolutionState);
}

void UNetworkAIComponent::ServerRPC_SpawnAIsAtRandomLocation_Implementation(AArena* _arena, EAIType _type, const int _count)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC Spawn AIs at random location", MAGENTA);
	//if (!_arena)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC spawn AIs at random location - Failed");
	//	return;
	//}
	//TSubclassOf<AAI_Base> _ref = _arena->GetArenaManager()->GetAIsRefs()->GetAIRefByType(_type);
	//_arena->SpawnAIsAtRandomLocation(_ref, _count);
}

void UNetworkAIComponent::ServerRPC_SetSpawner_Implementation(AArena* _arena, ASpawnerAI* _spawner, const TArray<int>& _AIsCount)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC set spawner", MAGENTA);
	//if (!_arena || !_spawner)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC set spawner - Failed");
	//	return;
	//}
	//TArray<TSubclassOf<AAI_Base>> _allRefs = _arena->GetArenaManager()->GetAIsRefs()->GetAllRefs();
	//_spawner->SetSpawner(_allRefs, _AIsCount);
}

void UNetworkAIComponent::ServerRPC_SettingAI_Implementation(AAI_Base* _AI, AArena* _arena)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC setting AI", MAGENTA);
	//if (!_AI || !_arena)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC setting AI - Failed");
	//	return;
	//}
	//UAIsSettingsDataAsset* _settingAI = _arena->GetArenaManager()->GetAISettings(_AI->GetType());
	//_AI->SetAI(_arena, _settingAI);
}
#pragma endregion 

#pragma region Perception-RPC
void UNetworkAIComponent::ServerRPC_PlayerDetected_Implementation(UPerceptionSystemComponent* _perception, AMainCharacter* _target)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC Player Detected", MAGENTA);
	//if (!_perception || !_target)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC player detected - Failed");
	//	return;
	//}
	//_perception->OnDetected().Broadcast(_target);
}

void UNetworkAIComponent::ServerRPC_Vigilance_Implementation(USightSystemComponent* _sight, const bool _vigilance)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC Vigilance", MAGENTA);
	//if (!_sight)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC vigilance - Failed");
	//	return;
	//}
	//_sight->OnVigilance().Broadcast(_vigilance);
}
#pragma endregion

#pragma region Projectile-RPC
void UNetworkAIComponent::ServerRPC_SetAllPoints_Implementation(AParaboleProjectile* _parabol, const TArray<FVector>& _allPoints)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC Set all points projectile", MAGENTA);
	//if (!_parabol)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC set all points - Failed");
	//	return;
	//}
	//_parabol->SetAllPoints(_allPoints);
}

void UNetworkAIComponent::ServerRPC_SetDirection_Implementation(ALineProjectile* _line, const FVector& _direction)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC Set Direction projectile", MAGENTA);
	//if (!_line)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC set direction - Failed");
	//	return;
	//}
	//_line->SetDirection(_direction);
}
#pragma endregion

#pragma region Life-RPC
void UNetworkAIComponent::ServerRpc_SetLife_Implementation(ULifeComponent* _lifeComp, const int _currentLife, const int _change, AActor* _damageSource)
{
	//LOG_COLOR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC set life", MAGENTA);
	//if (!_lifeComp)
	//{
	//	LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC set life - Failed");
	//	return;
	//}
	//_lifeComp->SetLife(_currentLife, _change, _damageSource);
}
#pragma endregion

#pragma region Camouflage
void UNetworkAIComponent::MulticastRPC_TransformMode_Implementation(UCamouflageComponent* _camouflage)
{
	if (!_camouflage)
	{
		LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC TransformMode - Failed");
		return;
	}
	_camouflage->TransformMode();
}

void UNetworkAIComponent::MulticastRPC_DisableTransformMode_Implementation(UCamouflageComponent* _camouflage)
{
	if (!_camouflage)
	{
		LOG_ERROR(NETWORK_AI_COMPONENT_USE_LOG, "ServerRPC DisableTransformMode - Failed");
		return;
	}
	_camouflage->DisableMesh();
}
#pragma endregion
