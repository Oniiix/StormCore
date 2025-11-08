// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerAI.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/AIsRefDataAsset.h"
#include "RobotHunter/AI/Settings/AIsSettingsDataAsset.h"
#include "RobotHunter/AI/Settings/ParalarvaSettingsDataAsset.h"
#include "RobotHunter/AI/Settings/TitanSettingsDataAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"

ASpawnerAI::ASpawnerAI()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = mesh;
	spawnPoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	spawnPoint->SetupAttachment(RootComponent);

	isInitializationGood = false;
}

void ASpawnerAI::InitSpawnerAI(AArena* _arena)
{
	/// Check arena availability
	arena = _arena;
	if (!arena)
	{
		LOG_ERROR(SPAWNER_USE_LOG, "Failed to init arena of " + ACTOR_NAME);
		return;
	}
	listAIsRef = arena->GetArenaManager()->GetAIsRefs()->GetAllRefs();
	if (listAIsRef.IsEmpty())
	{
		LOG_ERROR(SPAWNER_USE_LOG, "Failed to get list of AIs refs of " + ACTOR_NAME);
		return;
	}

	paralarvaSettings = arena->GetParalarvaSettings();
	titanSettings = arena->GetTitanSettings();

	/// Set location and rotation for spawning AIs
	spawnLocation = spawnPoint->GetComponentLocation();
	spawnRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), spawnLocation);
	spawnRotation.Pitch = 0;
	spawnRotation.Roll = 0;

	isInitializationGood = true;
}

void ASpawnerAI::AddAIToSpawn(const EAIType& _type)
{
	listAIsToSpawn.Add(_type);
}

void ASpawnerAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (spawning)
	{
		duration += DeltaSeconds;
		if (duration >= spawnRate)
		{
			duration = 0.f;
			spawning = false;
		}
	}

	if (!listAIsToSpawn.IsEmpty() && !spawning)
	{
		switch (listAIsToSpawn[0])
		{
		case Paralarva:
			SpawnAI(listAIsRef[0], paralarvaSettings);
			break;
		case Titan:
			SpawnAI(listAIsRef[2], titanSettings);
			break;
		}
		listAIsToSpawn.RemoveAt(0);
		spawning = true;
	}
}

void ASpawnerAI::SpawnAI(TSubclassOf<AAI_Base> _AIRef, TObjectPtr<UAIsSettingsDataAsset> _AISettings)
{
	/// Spawn AI on spawnPoint and register it in arena
	FActorSpawnParameters _param;
	_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAI_Base* _newAI = GetWorld()->SpawnActor<AAI_Base>(_AIRef, spawnLocation, spawnRotation, _param);
	_newAI->SetIsSpawned(true);
	_newAI->AttachToActor(arena, FAttachmentTransformRules::KeepWorldTransform);
	_newAI->SetSettings(_AISettings);
	arena->AddAI(_newAI);
}
