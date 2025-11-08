// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "EvolutionOnArenaDataTable.h"
#include "RobotHunter/AI/AIsRefDataAsset.h"
#include "RobotHunter/Game/GPE/Nest/SpawnerAI.h"
#include "RobotHunter/Game/GameMode/PrincipalGameMode.h"
#include "RobotHunter/Game/CustomGameState.h"
#include "RobotHunter/Network/Tree/TreeComponent.h"
#include "Component/AIRangePosition/AIRangePositionComponent.h"
#include "RobotHunter/Network/Component/CustomLODComponent.h"
#include "RobotHunter/Game/GPE/Nest/AlienDoor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "GameFramework/PlayerState.h"

AArena::AArena()
{
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	pin = CreateDefaultSubobject<UBillboardComponent>("Pin");
	RootComponent = pin;
	/// Use for relevant navMesh
	navInvoker = CreateDefaultSubobject< UNavigationInvokerComponent>("NavigationInvoker");
	AddOwnedComponent(navInvoker);
	/// Use for combat behaviour
	combatComponent = CreateDefaultSubobject<UArenaCombatComponent>("CombatComponent");
	AddOwnedComponent(combatComponent);
	/// Use for octree logic
	treeComponent = CreateDefaultSubobject<UTreeComponent>("Tree Component");
	AddOwnedComponent(treeComponent);
	treeComponent->SetIsCustomLoading(true);
	sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	sphere->SetupAttachment(RootComponent);
	/// Use for AI's combat positioning
	AIRangePositionComponent = CreateDefaultSubobject<UAIRangePositionComponent>("Range AI");
	AddOwnedComponent(AIRangePositionComponent);
	/// Use for performance
	customLODComponent = CreateDefaultSubobject< UCustomLODComponent>("CustomLOD");
	AddOwnedComponent(customLODComponent);

	/// Ensure replication
	bReplicates = true;
	SetReplicateMovement(true);

	/// Set sphere
	sphere->SetGenerateOverlapEvents(true);

	isInitializationGood = false;
	isLoaded = false;
}

#pragma region Start
void AArena::BeginPlay()
{
	Super::BeginPlay();
	InitArena();
}

void AArena::InitArena()
{
	//isLoaded = false;
	if (disable)
	{
		combatComponent->SetComponentTickEnabled(false);
		return;
	}

	/// Check sphere availability
	if (!sphere)
	{
		LOG_ERROR(ARENA_USE_LOG, "Sphere of " + ACTOR_NAME + " is null !!");
		return;
	}
	defaultCollisionProfileSphere = sphere->GetCollisionProfileName();
	sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AArena::SphereBeginOverlap);

	/// Check treeComponent availability
	if (!treeComponent)
	{
		LOG_ERROR(ARENA_USE_LOG, "TreeComponent of " + ACTOR_NAME + " is null !!");
		return;
	}
	/// Link octree to load system
	if (!alwaysLoaded)
		treeComponent->OnLoadStateChanged().AddUniqueDynamic(this, &AArena::Load);

	/// Check combatComponent availability
	if (!combatComponent)
	{
		LOG_ERROR(ARENA_USE_LOG, "CombatComponent of " + ACTOR_NAME + " is null !!");
		return;
	}
	combatComponent->OnFinishInitialization().AddUniqueDynamic(this, &AArena::CollectAIsInRange);
	combatComponent->OnPlayerEnterArena().AddUniqueDynamic(this, &AArena::StartCombat);
	const float _trackingRange = GetTrackingRange();

	/// Check navInvoker availability
	if (!navInvoker)
	{
		LOG_ERROR(ARENA_USE_LOG, "NavInvoker of " + ACTOR_NAME + " is null !!");
		return;
	}
	/// Set navigation invoker radius to breakRange, to remove useless navMesh
	navInvoker->SetGenerationRadii(_trackingRange, _trackingRange);

	/// Check customLODComponent availability
	if (!customLODComponent)
	{
		LOG_ERROR(ARENA_USE_LOG, "CustomLODComponent of " + ACTOR_NAME + " is null !!");
		return;
	}

	/// Check listArenaGate availability
	if (enterGate)
		enterGate->SetArena(this);
	else
		LOG_WARNING(ARENA_USE_LOG, "The Arena's enterGate of " + ACTOR_NAME + " is not referenced");
	if (exitGate)
		exitGate->CloseDoor();
	else
		LOG_WARNING(ARENA_USE_LOG, "The Arena's exitGate of " + ACTOR_NAME + " is not referenced");

	/// Check AI's settings availability
	if (!paralarvaSettings || !titanSettings)
	{
		LOG_ERROR(ARENA_USE_LOG, "AI's settings on " + ACTOR_NAME + " are bad referenced !!");
		return;
	}

	/// Server Side
	if (HasAuthority())
	{
		const bool _goodRegistration = RegisterArena();
		if (!_goodRegistration)
			return;

		combatComponent->InitArenaCombat();
	}
	else /// Client side
	{
		/// Get networkAI on playerController for RPC
		ACustomPlayerController* _playerController = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
		if (!_playerController)
		{
			LOG_ERROR(ARENA_USE_LOG, "Failed to get ACustomPlayerController in " + ACTOR_NAME);
			return;
		}
		networkAI = _playerController->NetworkAI();
	}

	/// Check listSpawners availability
	const int _countSpawners = listSpawners.Num();
	if (_countSpawners == 0 /*&& !disableSpawner*/)
	{
		LOG_ERROR(ARENA_USE_LOG, "The spawners list of " + ACTOR_NAME + " is empty !!");
		return;
	}
	for (int i = 0; i < _countSpawners; i++)
	{
		if (listSpawners[i] == nullptr)
		{
			LOG_ERROR(ARENA_USE_LOG, "The spawners list of " + ACTOR_NAME + " is invalid !!");
			return;
		}
		/// Set & bind 
		listSpawners[i]->InitSpawnerAI(this);
		listSpawners[i]->SetVisibility(alwaysLoaded);
	}
	spawnerLastIndex = _countSpawners - 1;

	isInitializationGood = true;

	/// Load forever server side
	if (HasAuthority() && alwaysLoaded)
		Load(true);
}

bool AArena::RegisterArena()
{
	/// Check ArenaManager availability
	APrincipalGameMode* _gameMode = GetWorld()->GetAuthGameMode<APrincipalGameMode>();
	ACustomGameState* _gameState = GetWorld()->GetGameState<ACustomGameState>();
	if (!_gameMode)
	{
		LOG_ERROR(ARENA_USE_LOG, "Failed to get CustomGameModeBase !!");
		return false;
	}
	/// Stop if gameMode state is MAIN_MENU or HUB
	if (!_gameState->IsInGame())
		return false;
	arenaManager = _gameMode->GetArenaManager();
	if (!arenaManager)
	{
		LOG_ERROR(ARENA_USE_LOG, "Failed to get UArenaManager !!");
		return false;
	}
	arenaManager->AddArena(this);
	return true;
}

void AArena::CollectAIsInRange()
{
	/// Update sphere collision to get AIs actor in 1 frame
	sphere->SetCollisionProfileName(sphereCollisionProfileAI.Name);
	/// Disable sphere collision
	sphere->SetCollisionProfileName(defaultCollisionProfileSphere);
	sphere->SetGenerateOverlapEvents(false);

	sphere->OnComponentBeginOverlap.RemoveDynamic(this, &AArena::SphereBeginOverlap);
	onFinishToCollectAI.Broadcast();
}

void AArena::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isInitializationGood)
		return;

	AAI_Base* _AI = Cast<AAI_Base>(OtherActor);
	if (_AI)
	{
		/// Register AI server side
		AddAI(_AI);
		/// Register client side by OnRep_ListAIs
	}
	UCoverComponent* _cover = OtherActor->GetComponentByClass<UCoverComponent>();
	if (_cover)
	{
		listCover.Add(_cover);
		_cover->Init(AIRangePositionComponent);
	}
}
#pragma endregion

#pragma region Tick
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebug();

	if (!isInitializationGood)
		return;
}

bool AArena::ShouldTickIfViewportsOnly() const
{
	return true;
}
#pragma endregion

#pragma region AIs
void AArena::AddAI(AAI_Base* _newAI)
{
	/// Set
	_newAI->SetAI(this);

	/// Register
	RegisterInAILists(_newAI);
	if (combatComponent->IsOnAlert())
		combatComponent->AlertAI(_newAI);

	/// InitAnimInstance
	_newAI->OnCombat().AddUniqueDynamic(combatComponent, &UArenaCombatComponent::ArenaAlert);
	_newAI->GetAttackComponent()->OnChangeTarget().AddUniqueDynamic(combatComponent, &UArenaCombatComponent::OnAIChangeTarget);
	_newAI->OnPlayDeath().AddUniqueDynamic(this, &AArena::CheckAI);
}

void AArena::RegisterInAILists(AAI_Base* _newAI)
{
	uint32 _Id = _newAI->GetUniqueID();
	if (mapAIs.Contains(_Id))
		return;

	mapAIs.Add(_Id, _newAI);
	listAIs.Add(_newAI);

	switch (_newAI->GetType())
	{
	case EAIType::Paralarva:
		countParalarvas += 1;
		break;
	case EAIType::Trapper:
		countTrappers += 1;
		break;
	case EAIType::Titan:
		countTitans += 1;
		break;
	}

	/// Start Reflexion
	_newAI->EnableAI(isLoaded);
}

void AArena::RemoveAIFromLists(AAI_Base* _AI)
{
	uint32 _Id = _AI->GetUniqueID();
	if (!mapAIs.Contains(_Id))
		return;

	mapAIs.Remove(_Id);
	listAIs.Remove(_AI);

	switch (_AI->GetType())
	{
	case Paralarva:
		onParalarvaDie.Broadcast();
		countParalarvas += -1;
		break;
	case Trapper:
		countTrappers += -1;
		/// All trappers are dead
		if (countTrappers == 0)
			ClearArena(true);
		break;
	case Titan:
		countTitans += -1;
		break;
	}

	if (disable && mapAIs.IsEmpty())
		RemoveUseless();
}

void AArena::RemoveUseless()
{
	LOG_COLOR(ARENA_USE_LOG, "REMOVE USELESS", BLACK);
	//combatComponent->DestroyComponent();
	AIRangePositionComponent->DestroyComponent();
	navInvoker->DestroyComponent();
	customLODComponent->DestroyComponent();
}

void AArena::CheckAI(AAI_Base* _AI)
{
	/// Unregister _AI
	RemoveAIFromLists(_AI);
	combatComponent->AIDie(_AI);
	/// Respawn if not cleared
	if (_AI->GetType() != EAIType::Trapper && !disable)
		DispatchAIBetweenSpawner(_AI);

	/// Unbind _AI
	_AI->OnDie().RemoveAll(this);
	_AI->OnCombat().RemoveAll(combatComponent);
	_AI->InitDie();
}

void AArena::ClearArena(const bool _giveBonus)
{
	LOG_COLOR(ARENA_USE_LOG, ACTOR_NAME + " is cleared. No more spawn", BLUE);
	disable = true;

	/// Kill all remaining AIs
	const int _listAIsCount = listAIs.Num() - 1;
	for (int i = _listAIsCount; i >= 0; i--) /// Reverse because killing AI update list
	{
		AAI_Base* _AI = listAIs[i];
		if (!_AI)
			continue;
		ULifeComponent* _AILifeComponent = _AI->GetLifeComponent();
		if (!_AILifeComponent)
		{
			_AI->PlayDeath();
			continue;
		}
		const int _AILife = _AILifeComponent->GetCurrentLife();
		_AILifeComponent->Damage(_AILife * 2, this); // Be sure to kill
	}

	/// Disable spawners
	for (int i = 0; i <= spawnerLastIndex; i++)
		listSpawners[i]->SetActorTickEnabled(false);

	/// Open arena
	if (exitGate)
	{
		exitGate->OpenDoor();
		exitGate->ShowWidget();
		onClearArena.Broadcast(exitGate, _giveBonus);
	}
}

void AArena::EndSequencer()
{
	/// Hide or not trapper
	const int _listAIsCount = listAIs.Num();
	for (int i = 0; i < _listAIsCount; i++)
	{
		if (listAIs[i]->GetType() == EAIType::Trapper)
		{
			AAI_Hunter* _trapper = Cast<AAI_Hunter>(listAIs[i]);
			_trapper->SetCanStartCamouflage(hideTrapper);
		}
	}

	/// Engage combat
	combatComponent->ArenaAlert();
}

void AArena::StartCombat(FUniqueNetIdRepl _playerNetID)
{
	/// Engage combat
	if (!useSequencer)
		combatComponent->ArenaAlert();
}

void AArena::CloseArena()
{
	if (disable)
		return;
	LOG_COLOR(ARENA_USE_LOG, "CLOSE ARENA", WHITE);

	if (enterGate)
	{
		enterGate->CloseDoor();
		enterGate->HideWidget();
	}
	if (exitGate)
		exitGate->HideWidget();
	onCloseArena.Broadcast();
}

void AArena::SetupPlayerCamera()
{
	AMainCharacter* _player = Cast<AMainCharacter>(GETFPC->GetCharacter());
	_player->GetCameraComponent()->GetCamera()->SetWorldTransform(arenaCameraTransform);
}

void AArena::FillMissingAIs()
{
	/// Dispath missing paralarva
	const int _missingParalarvaCount = maxParalarvaCount - countParalarvas;
	if (_missingParalarvaCount > 0)
	{
		for (int i = 0; i < _missingParalarvaCount; i++)
		{
			listSpawners[spawnerIndex]->AddAIToSpawn(EAIType::Paralarva);
			spawnerIndex = spawnerIndex == spawnerLastIndex ? 0 : spawnerIndex + 1;
		}
	}
	/// Dispath missing titan
	const int _missingTitanCount = maxTitanCount - countTitans;
	if (_missingTitanCount > 0)
	{
		for (int i = 0; i < _missingTitanCount; i++)
		{
			listSpawners[spawnerIndex]->AddAIToSpawn(EAIType::Titan);
			spawnerIndex = spawnerIndex == spawnerLastIndex ? 0 : spawnerIndex + 1;
		}
	}
}

void AArena::DispatchAIBetweenSpawner(AAI_Base* _AI)
{
	FTimerHandle _timerAICooldown;
	FTimerDelegate _DelegateAICooldown;
	_DelegateAICooldown.BindUFunction(listSpawners[spawnerIndex], FName("AddAIToSpawn"), _AI->GetType());
	if (_AI->GetType() == EAIType::Paralarva)
		GetWorld()->GetTimerManager().SetTimer(_timerAICooldown, _DelegateAICooldown, paralarvaCooldown, false);
	else
		GetWorld()->GetTimerManager().SetTimer(_timerAICooldown, _DelegateAICooldown, titanCooldown, false);

	spawnerIndex = spawnerIndex == spawnerLastIndex ? 0 : spawnerIndex + 1;
}
#pragma endregion

#pragma region Load
void AArena::Load(const bool _loaded)
{
	/// Shutdown for bad init or repetition
	if (!isInitializationGood || _loaded == isLoaded)
		return;

	/// Check for always loaded
	isLoaded = _loaded || alwaysLoaded;
	const FString& _text = ACTOR_NAME + (isLoaded ? " is loaded" : " is unloaded");
	LOG_COLOR(ARENA_USE_LOG, _text, CYAN);

	/// Server Manage loading
	HasAuthority() ? LoadManagement(isLoaded) : networkAI->ServerRPC_LoadManagement(this, isLoaded);

	Loading();
}

void AArena::Loading()
{
	/// Enable/Disable IAs
	int _AIFlag = 0;
	for (TPair<uint32, AAI_Base*>& _element : mapAIs)
	{
		/// Enable AIs delayed to avoid losing frame
		if (!_element.Value)
			continue;
		_AIFlag++;
		const int _modulo = (_AIFlag / 5) + 1; /// Group delayed AI
		FTimerHandle _timerSpawnAI;
		FTimerDelegate _DelegateSpawnAI;
		_DelegateSpawnAI.BindUFunction(this, FName("DelayedEnableAI"), _element.Value);
		GetWorld()->GetTimerManager().SetTimer(_timerSpawnAI, _DelegateSpawnAI, DELTA_TIME * _modulo, false);
	}

	/// Display/Hide listSpawners
	const int _countSpawners = listSpawners.Num();
	for (int i = 0; i < _countSpawners; i++)
		listSpawners[i]->SetVisibility(isLoaded);
}

void AArena::DelayedEnableAI(AAI_Base* _AI)
{
	_AI->EnableAI(isLoaded);
}

void AArena::LoadManagement(const bool _isLoaded)
{
	/// Token system (clamp min to zero to secure)
	const int _token = _isLoaded ? 1 : -1;
	token = token + _token < 0 ? 0 : token + _token;

	// if error here, just restart (skill issue)

	if (!arenaManager)
	{
		LOG_ERROR(ARENA_USE_LOG, "arenaManager is null in " + ACTOR_NAME);
		return;
	}

	/// Arena unloaded host and client side
	if (token == 0)
	{
	}
}

void AArena::SpawnAIsAtRandomLocation(const TSubclassOf<AAI_Base>& _ref, const int _count)
{
	if (!_ref)
	{
		LOG_ERROR(ARENA_USE_LOG, "AI's ref to spawn is null : abort spawn");
		return;
	}

	/// Spawn AIs delayed to avoid losing frame
	int _flag = 0;
	for (int i = 0; i < _count; i++)
	{
		_flag++;
		FTimerHandle _timerSpawnAI;
		FTimerDelegate _DelegateSpawnAI;
		_DelegateSpawnAI.BindUFunction(this, FName("DelayedSpawnAI"), _ref);
		GetWorld()->GetTimerManager().SetTimer(_timerSpawnAI, _DelegateSpawnAI, DELTA_TIME * _flag, false);
	}
}

void AArena::DelayedSpawnAI(const TSubclassOf<AAI_Base>& _ref)
{
	const FVector& _location = RandomLocationInArena();
	const float _randYaw = RANDOM_FLOAT_RANGE(0, 360);
	const FRotator& _rotation = FRotator(0.f, _randYaw, 0.f);
	FActorSpawnParameters _param;
	_param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAI_Base* _newAI = GetWorld()->SpawnActor<AAI_Base>(_ref, _location, _rotation, _param);
	if (_newAI)
	{
		/// Register IA
		AddAI(_newAI);
	}
}
#pragma endregion

#pragma region Replication
void AArena::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AArena, disable);
	DOREPLIFETIME(AArena, alwaysLoaded);
	DOREPLIFETIME(AArena, token);
	DOREPLIFETIME(AArena, spawnersEnabled);
	DOREPLIFETIME(AArena, countParalarvas);
	DOREPLIFETIME(AArena, countTrappers);
	DOREPLIFETIME(AArena, countTitans);
}
#pragma endregion

#pragma region Utils
void AArena::DrawDebug()
{
	if (!drawDebug)
		return;

	/// Draw simple circle for arenaRange debug
	DRAW_CIRCLE(GetActorLocation(), arenaRange, debugColor, GetActorForwardVector(), GetActorRightVector());

	/// Draw dotted circle for breakRange debug
	for (int i = 0; i < 360; i++)
	{
		const float _angle = DEG_TO_RAD(i);
		const float _x = COS(_angle, breakRange);
		const float _y = SIN(_angle, breakRange);
		const FVector& _direction = FVector(_x, _y, 0);

		DRAW_CIRCLE_ARC(GetActorLocation(), breakRange, debugColor, _direction, 0.0025f);
	}
}

FVector AArena::RandomLocationInArena()
{
	FVector _res = GetActorLocation();
	/// Check NavigationSystem availability
	UNavigationSystemV1* _nav = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!_nav)
	{
		LOG_ERROR(ARENA_USE_LOG, "Failed to find navigation system on " + ACTOR_NAME);
		return _res;
	}

	/// Get available location
	FNavLocation _point;
	for (int i = 0; i < 100; i++)
	{
		const bool _success = _nav->GetRandomReachablePointInRadius(_res, arenaRange, _point);
		if (_success)
		{
			_res = _point.Location;
			break;
		}
	}
	return _res;
}
#pragma endregion

#if WITH_EDITOR
void AArena::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Clamp min breakRange to arenaRange + 100, and set navInvoker radius
	const bool _RangeValueChange = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AArena, arenaRange)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(AArena, breakRange);
	if (_RangeValueChange)
	{
		if (arenaRange > breakRange - 100)
			breakRange = arenaRange + 100;
		sphere->SetSphereRadius(breakRange);
		const float _trackingRange = GetTrackingRange();
		navInvoker->SetGenerationRadii(_trackingRange, _trackingRange);
		customLODComponent->SetDistance(_trackingRange * 1.5f);
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
