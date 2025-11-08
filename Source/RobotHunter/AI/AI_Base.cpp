// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Base.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Arena/Arena.h"
#include "Components/CapsuleComponent.h"

AAI_Base::AAI_Base(TEnumAsByte<EAIType> _type)
{
	PrimaryActorTick.bCanEverTick = false;

	/// Brain
	stateTreeComponent = CreateDefaultSubobject<UAIStateTreeComponent>("StateTreeComponent");
	AddOwnedComponent(stateTreeComponent);
	stateTreeComponent->SetStartLogicAutomatically(false);
	/// Life
	lifeComponent = CreateDefaultSubobject<ULifeComponent>("LifeComponent");
	AddOwnedComponent(lifeComponent);
	/// Status
	statusComponent = CreateDefaultSubobject<UStatusComponent>("StatusComponent");
	AddOwnedComponent(statusComponent);
	/// Hurtbox
	hurtbox = CreateDefaultSubobject<UCapsuleComponent>("Hurtbox");
	hurtbox->SetupAttachment(RootComponent);
	weakPoint = CreateDefaultSubobject<UWeakPointComponent>("WeakPoint");
	weakPoint->SetupAttachment(GetMesh());
	/// UI
	lifeBarComponent = CreateDefaultSubobject<ULifebarWidgetComponent>("LifebarWidget");
	lifeBarComponent->SetupAttachment(RootComponent);
	debugInfo = CreateDefaultSubobject<UAIDebugInfoWidgetComponent>("DebugInfo");
	debugInfo->SetupAttachment(RootComponent);
	/// Tracking
	damageComponent = CreateDefaultSubobject<UDamageSystemComponent>("DamageComponent");
	AddOwnedComponent(damageComponent);
	trackingComponent = CreateDefaultSubobject<UTrackingSystemComponent>("TrackingComponent");
	AddOwnedComponent(trackingComponent);
	/// Movement
	movementComponent = CreateDefaultSubobject<UAIMovementComponent>("MovementComponent");
	AddOwnedComponent(movementComponent);
	movableComponent = CreateDefaultSubobject<UMovableComponent>("MovableComponent");
	AddOwnedComponent(movableComponent);
	/// Attack
	attackComponent = CreateDefaultSubobject<UAttackComponent>("AttackComponent");
	AddOwnedComponent(attackComponent);
	offsetShoot = CreateDefaultSubobject<USceneComponent>("OffsetShoot");
	offsetShoot->SetupAttachment(RootComponent);
	BoxCQC = CreateDefaultSubobject<UBoxComponent>("BoxCQC");
	BoxCQC->SetupAttachment(RootComponent);

	type = _type;
	loadingToken = 0;
	isEnable = false;
	isDead = false;
	isSet = false;
	isSpawned = false;
	isFighting = false;
	disableAllPerception = false;
	isInitializationGood = false;
}

#pragma region Start
void AAI_Base::BeginPlay()
{
	Super::BeginPlay();

	InitAI();

	//if (arena && arena->GetMapAIs().Contains(GetUniqueID()))
	//	EnableAI(arena->IsLoaded());
}

void AAI_Base::InitAI()
{
	/// Ensure replication
	bReplicates = true;
	SetReplicateMovement(true);
	ComponentReplication();

	/// Check stateTreeComponent validity
	if (!stateTreeComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "StateTreeComponent in " + ACTOR_NAME + " is null !!");
		return;
	}
	if (stateTreeComponent->IsRunning())
		stateTreeComponent->StopLogic("Don't start on Init");

	/// Check PerceptionComponents validity
	if (!damageComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "Dammage components in " + ACTOR_NAME + " is invalid !!");
		return;
	}

	/// Check trackingComponent validity
	if (!trackingComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "Tracking component in " + ACTOR_NAME + " is null !!");
		return;
	}

	/// Check lifeComponent validity
	if (!lifeComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "lifeComponent component in " + ACTOR_NAME + " is null !!");
		return;
	}
	lifeComponent->OnDie().AddUniqueDynamic(this, &AAI_Base::PlayDeath);
	lifeComponent->OnLifeChangeByDamage().AddUniqueDynamic(this, &AAI_Base::SlowAI);

	/// Check lifebarComponent validity
	if (!lifeBarComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "lifebar component in " + ACTOR_NAME + " is null !!");
		return;
	}
	lifeBarComponent->InitLifebar(lifeComponent);

	/// Check debugInfo validity
	if (!debugInfo)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "AIDebugInfo component in " + ACTOR_NAME + " is null !!");
		return;
	}
	debugInfo->InitAIDebugInfo();

	/// Check movementComponent validity
	if (!movementComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "movementComponent component in " + ACTOR_NAME + " is null !!");
		return;
	}
	movementComponent->OnMove().AddUniqueDynamic(this, &AAI_Base::MoveAnimation);

	/// Check attackComponent validity
	if (!attackComponent)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "attackComponent component in " + ACTOR_NAME + " is null !!");
		return;
	}

	/// Check offsetShoot validity
	if (!offsetShoot)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "offsetShoot component in " + ACTOR_NAME + " is null !!");
		return;
	}

	/// Check BoxCQC validity
	if (!BoxCQC)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "BoxCQC component in " + ACTOR_NAME + " is null !!");
		return;
	}

	/// Check hurtbox validity
	if (!hurtbox)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "hurtbox component in " + ACTOR_NAME + " is null !!");
		return;
	}

	/// Check weakPoint validity
	if (!weakPoint)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "WeakPoint component in " + ACTOR_NAME + " is null !!");
		return;
	}

	skeletalMesh = GetMesh();

	/// Owner for RPC
	SetOwner(GetWorld()->GetFirstPlayerController());

	/// Bind tracking
	trackingComponent->OnDetection().AddUniqueDynamic(this, &AAI_Base::Detection);
	trackingComponent->OnTargetLost().AddUniqueDynamic(this, &AAI_Base::TargetLost);

	RootComponent->SetVisibility(false, true);
	UpdateComponent(false);

	/// Allow rotation managment
	bUseControllerRotationYaw = false;

	startLocation = GetActorLocation();
	isInitializationGood = true;
}

void AAI_Base::SetAI(AArena* _arena)
{
	if (!isInitializationGood)
		return;

	arena = _arena;

	/// Set AI by _settings (Carefull Init() order)
#pragma region Settings
	/// StateTree
	stateTreeComponent->SetStateTreeRef(settings->GetStateTreeRef());
	stateTreeComponent->InitStateTreeComponent();
	/// Life
	lifeComponent->SetMaxLife(settings->GetMaxLife());
	lifeComponent->SetStartLife(settings->GetStartLife());
	lifeComponent->InitLife();
	weakPoint->SetDamageMultiplier(settings->GetWeakPointDamageMultiplier());
	/// Movement
	movementComponent->SetWalkSpeed(settings->GetWalkSpeed());
	movementComponent->SetRunSpeed(settings->GetRunSpeed());
	movementComponent->SetAcceleration(settings->GetAcceleration());
	movementComponent->SetRotationSpeed(settings->GetRotationSpeed());
	movementComponent->SetLayersGround(settings->GetLayersGround());
	rotationSpeed = settings->GetRotationSpeed();
	movementComponent->SetDrawDebug(settings->GetDrawDebugMove());
	slowedDelay = settings->GetSlowedDelay();
	maxSlow = settings->GetMaxSlow();
	maxDamageSlow = settings->GetMaxDamageSlow();
	movementComponent->InitAIMovement();
	/// Damage
	damageComponent->InitPerception();
	/// Tracking
	trackingComponent->SetDisableTracking(settings->GetDisableTracking());
	trackingComponent->SetDrawDebug(settings->GetDrawDebugTracking());
	trackingComponent->InitTracking();
	/// Attack
	attackComponent->SetDisableAttack(settings->GetDisableAttack());
	attackComponent->SetListRange(settings->GetListRange());
	attackComponent->SetPushSnapPower(settings->GetPushSnapPower());
	attackComponent->SetMoveDistance(settings->GetMoveDistance());
	attackComponent->SetImprecisionFactor(settings->GetImprecisionFactor());
	attackComponent->InitAttack();
#pragma endregion

	isSet = true;
	LOG_COLOR(AI_BASE_USE_LOG, ACTOR_NAME + " is set", CYAN);
	onSet.Broadcast();
}
#pragma endregion

#pragma region Perception
void AAI_Base::Detection()
{
	/// Not already in combat
	if (!isFighting)
	{
		LOG_COLOR(AI_BASE_USE_LOG, ACTOR_NAME + " on combat", ORANGE);
		onCombat.Broadcast();
	}
}

void AAI_Base::TargetLost()
{
	LOG_COLOR(AI_BASE_USE_LOG, ACTOR_NAME + " lost Player", PINK);
	SetIsFighting(false);
}
#pragma endregion

#pragma region Combat
void AAI_Base::SetIsFighting(const bool _value)
{
	/// On_Rep client
	isFighting = _value;
	UpdateMode();
}

void AAI_Base::UpdateMode()
{
	FString _msg = ACTOR_NAME + (isFighting ? " is fighting" : " is chill");
	LOG_COLOR(AI_BASE_USE_LOG, _msg, PINK);
	trackingComponent->SetComponentTickEnabled(isFighting);
}

FUniqueNetIdRepl AAI_Base::StartToFight()
{
	SetIsFighting(true);
	isSpawned = false;

	FUniqueNetIdRepl _playerId = attackComponent->GetPlayerIdWithHighestThreat();
	AMainCharacter* _player = attackComponent->GetCharacterByNetId(_playerId);
	if (!_player)
	{
		LOG_WARNING(true, "Failed to get player with highest threat on " + ACTOR_NAME + ". No target assigned");
		return FUniqueNetIdRepl();
	}
	trackingComponent->SetTarget(_player);

	return _playerId;
}

void AAI_Base::StunAI(const float _stunTime)
{
	/// Enter in combat state if not
	if (!isFighting)
		StartToFight();

	onStun.Broadcast(_stunTime);
}

void AAI_Base::SlowAI(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource)
{
	const float _absDamage = abs(_damage);
	if (_absDamage >= maxDamageSlow)
	{
		slowedStatus.value = 100.f - maxSlow;
	}
	else
	{
		const float _damagePercent = _absDamage / maxDamageSlow;
		slowedStatus.value = 100.f - (maxSlow * _damagePercent);
	}
	slowedStatus.duration = slowedDelay;

	statusComponent->AddStatus(this, slowedStatus);
}

void AAI_Base::PlayDeath()
{
	if (isDead)
		return;

	isDead = true;
	Ragdoll();
	attackComponent->ResetAttack();
	onPlayDeath.Broadcast(this);
}

void AAI_Base::Ragdoll()
{
	/// Disable Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	hurtbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weakPoint->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetBoxCQC()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	skeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	/// Enable physic body
	skeletalMesh->SetSimulatePhysics(true);
	skeletalMesh->WakeAllRigidBodies();
	skeletalMesh->bBlendPhysics = true;
	skeletalMesh->AddImpulse(GetCharacterMovement()->PendingLaunchVelocity * 10, FName("pelvis"), true);
}

void AAI_Base::InitDie()
{
	onDie.Broadcast(this);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	FTimerHandle _timerRagdoll;
	GetWorldTimerManager().SetTimer(_timerRagdoll, this, &AAI_Base::Die, ragdollDuration);

	/// Destroy useless component
	lifeComponent->DestroyComponent();
	lifeBarComponent->DestroyComponent();
	trackingComponent->DestroyComponent();
	movementComponent->DestroyComponent();
	attackComponent->DestroyComponent();
	BoxCQC->DestroyComponent();
	hurtbox->DestroyComponent();
	weakPoint->DestroyComponent();
}

void AAI_Base::Die()
{
	/// Disable ragdoll for opti
	skeletalMesh->PutAllRigidBodiesToSleep();
	//skeletalMesh->SetSimulatePhysics(false);
	//skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(lifeSpan);
	onFade.Broadcast();
}
#pragma endregion

#pragma region StateTree
void AAI_Base::EnableAI(const bool _value)
{
	/// Cut for bad init or repetition
	if (!isInitializationGood || isEnable == _value)
		return;

	isEnable = _value;
	onEnable.Broadcast(isEnable);

	const FString& _msg = ACTOR_NAME + (isEnable ? " is enable" : " is disable");
	LOG_COLOR(AI_BASE_USE_LOG, _msg, CYAN);

	/// Update visibility
	RootComponent->SetVisibility(isEnable, true);

	/// Update behaviour
	const int _token = isEnable ? 1 : -1;
	/// Check customPlayerController availability
	ACustomPlayerController* _pc = Cast<ACustomPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!_pc)
	{
		LOG_ERROR(AI_BASE_USE_LOG, "Failed to get ACustomPlayerController in " + ACTOR_NAME);
		return;
	}
	/// Update on server
	HasAuthority() ? EnableBehaviour(_token) : _pc->NetworkAI()->ServerRPC_AIBehaviour(this, _token);
}

void AAI_Base::EnableBehaviour(const int _token)
{
	/// Token system (clamp min to zero to secure)
	loadingToken = loadingToken + _token < 0 ? 0 : loadingToken + _token;

	/// Check Host/Client loaded
	if (loadingToken > 0)
	{
		/// Not already running
		if (stateTreeComponent->GetStateTreeRunStatus() != EStateTreeRunStatus::Running)
		{
			LOG_COLOR(AI_BASE_USE_LOG, ACTOR_NAME + " Enable Behaviour", BLUE);
			/// Start "brain"
			stateTreeComponent->StartLogic();
			SetActorEnableCollision(true);
			UpdateComponent(true);
		}
	}
	else
	{
		LOG_COLOR(AI_BASE_USE_LOG, ACTOR_NAME + " Disable Behaviour", ORANGE);
		/// Place AI inside arenaRange if needed
		const FVector& _arenalocation = arena->GetActorLocation();
		const FVector& _AILocation = GetActorLocation();
		const int _arenaRange = arena->GetArenaRange();
		if (FVector::Dist(_arenalocation, _AILocation) > _arenaRange)
		{
			const FVector& _inArenaLocation = arena->RandomLocationInArena();
			SetActorLocation(_inArenaLocation);
		}

		/// Stop "brain"
		stateTreeComponent->StopLogic("Disable IA");
		UpdateComponent(false);
		SetActorEnableCollision(false);
	}
}

void AAI_Base::UpdateComponent(const bool _value)
{
	GetMesh()->bPauseAnims = !_value;
	GetMesh()->SetComponentTickEnabled(_value);
	lifeBarComponent->SetWidgetVisibility(_value);
	if (debugInfo)
		debugInfo->SetWidgetVisibility(_value);
	trackingComponent->SetComponentTickEnabled(_value);
	attackComponent->SetComponentTickEnabled(_value);
	GetCharacterMovement()->SetComponentTickEnabled(_value);
}
#pragma endregion

#pragma region Replication
void AAI_Base::ComponentReplication()
{
	/// Enable replication on all components
	for (UActorComponent* _actorComponent : GetComponents())
	{
		_actorComponent->SetIsReplicated(true);
	}
	/// Disable replication of Root, SkeletalMesh, LifeBar to display/hide locally
	RootComponent->SetIsReplicated(false);
	GetMesh()->SetIsReplicated(false);
	lifeBarComponent->SetIsReplicated(false);
	if (debugInfo)
		debugInfo->SetIsReplicated(false);
}

void AAI_Base::OnRep_UpdateArena()
{
	if (arena)
		arena->RegisterInAILists(this);
}

void AAI_Base::OnRep_IsFighting()
{
	UpdateMode();
}

void AAI_Base::OnRep_IsDead()
{
	if (isDead)
		onDie.Broadcast(this);
}

void AAI_Base::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAI_Base, loadingToken);
	DOREPLIFETIME(AAI_Base, arena);
	DOREPLIFETIME(AAI_Base, isDead);
	DOREPLIFETIME(AAI_Base, isFighting);
	DOREPLIFETIME(AAI_Base, isSet);
}

void AAI_Base::SetTickIntervalForActorAndComponents(float _newTickInterval)
{
	// Modify Tick interval
	// Apply same interval on all components
	for (UActorComponent* _actorComponent : GetComponents())
	{
		if (_actorComponent->IsRegistered() && _actorComponent->PrimaryComponentTick.bCanEverTick)
			_actorComponent->PrimaryComponentTick.TickInterval = _newTickInterval;
	}
}
#pragma endregion

void AAI_Base::MoveAnimation(int _axis)
{
	onMove.Broadcast(_axis);
}
