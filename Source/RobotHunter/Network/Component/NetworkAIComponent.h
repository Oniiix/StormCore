// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/Utils/ENUM_Projectile.h"
#include "NetworkAIComponent.generated.h"

#define NETWORK_AI_COMPONENT_USE_LOG false

class AAI_Base;
class AArena;
class ASpawnerAI;
class ULifeComponent;
class UPerceptionSystemComponent;
class USightSystemComponent;
class UListenSystemComponent;
class ASphereSound;
class UAttackComponent;
class ALineProjectile;
class ULifeComponent;
class UCamouflageComponent;

/// <summary>
/// Component for all AI's networking
/// </summary>
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UNetworkAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNetworkAIComponent();

	/// AI-RPC
	UFUNCTION(Server, Unreliable)
	void ServerRPC_AIBehaviour(AAI_Base* _AI, const int _token);

	UFUNCTION(Server, Unreliable)
	void ServerRPC_LoadManagement(AArena* _arena, const bool _isLoaded);

	UFUNCTION(Server, Unreliable)
	void ServerRPC_EvolutionManagement(AArena* _arena, const int _currentEvolutionState);
	UFUNCTION(Server, Unreliable)
	void ServerRPC_SetSpawner(AArena* _arena, ASpawnerAI* _spawner, const TArray<int>& _AIsCount);
	UFUNCTION(Server, Unreliable)
	void ServerRPC_SpawnAIsAtRandomLocation(AArena* _arena, EAIType _type, const int _count);
	UFUNCTION(Server, Unreliable)
	void ServerRPC_SettingAI(AAI_Base* _AI, AArena* _arena);

	/// Perception-RPC
	UFUNCTION(Server, Unreliable)
	void ServerRPC_PlayerDetected(UPerceptionSystemComponent* _perception, AMainCharacter* _target);
	UFUNCTION(Server, Unreliable)
	void ServerRPC_Vigilance(USightSystemComponent* _sight, const bool _vigilance);

	/// Projectile-RPC
	UFUNCTION(Server, Unreliable)
	void ServerRPC_SetAllPoints(AParaboleProjectile* _parabol, const TArray<FVector>& _allPoints);
	UFUNCTION(Server, UnReliable)
	void ServerRPC_SetDirection(ALineProjectile* _line, const FVector& _direction);

	/// Life-RPC
	UFUNCTION(Server, Unreliable)
	void ServerRpc_SetLife(ULifeComponent* _lifeComp, const int _currentLife, const int _change, AActor* _damageSource);

	/// Camouflage-RPC
	UFUNCTION(NetMulticast, UnReliable) void MulticastRPC_TransformMode(UCamouflageComponent* _camouflage);
	UFUNCTION(NetMulticast, UnReliable) void MulticastRPC_DisableTransformMode(UCamouflageComponent* _camouflage);
};
