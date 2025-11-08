// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/Tracking/TrackingSystemComponent.h"
#include "Component/Perception/DamageSystemComponent.h"
#include "Component/Movement/AIMovementComponent.h"
#include "Component/StateTree/AIStateTreeComponent.h"
#include "Component/Life/LifeComponent.h"
#include "Component/Life/WeakPointComponent.h"
#include "Component/Attack/AttackComponent.h"
#include "Component/Widget/LifebarWidgetComponent.h"
#include "Component/Widget/AIDebugInfoWidgetComponent.h"
#include "Settings/AIsSettingsDataAsset.h"
#include "RobotHunter/CCC/Character/Components/Status/StatusComponent.h"
#include "RobotHunter/AI/Animation/AIBaseAnimInstance.h"
#include "RobotHunter/Game/GPE/Components/MovableComponent/MovableComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AI_Base.generated.h"

#define AI_BASE_USE_LOG false 
#define NAME_AI_BASE "AI_Base"
#define NAME_AI_PARALARVA "AI_Paralarva"
#define NAME_AI_PENNANT "AI_Pennant"
#define NAME_AI_TRAPPER "AI_Trapper"
#define NAME_AI_TORPEDO "AI_Torpedo"
#define NAME_AI_TITAN "AI_Titan"

class AArena;

/**
 * The base abstract class for AI entities
 */
UCLASS(ABSTRACT)
class ROBOTHUNTER_API AAI_Base : public ACharacter
{
	GENERATED_BODY()

protected:
#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnable, bool, enable);
	FOnEnable onEnable;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove, int, _axis);
	FOnMove onMove;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombat);
	FOnCombat onCombat;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStun, float, _stunTime);
	FOnStun onStun;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDie, AAI_Base*, _this);
	FOnDie onDie;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFade);
	UPROPERTY(BlueprintAssignable)
	FOnFade onFade;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayDeath, AAI_Base*, _this);
	FOnPlayDeath onPlayDeath;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSet);
	FOnSet onSet;
#pragma endregion

#pragma region Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UAIStateTreeComponent> stateTreeComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UStatusComponent> statusComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<ULifeComponent> lifeComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UCapsuleComponent> hurtbox = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UWeakPointComponent> weakPoint = nullptr;
	/// UI
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UAIDebugInfoWidgetComponent> debugInfo = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<ULifebarWidgetComponent> lifeBarComponent = nullptr;
	/// Tracking
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UTrackingSystemComponent> trackingComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UDamageSystemComponent> damageComponent = nullptr;
	/// Movement
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UAIMovementComponent> movementComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UMovableComponent> movableComponent = nullptr;
	/// Attack
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UAttackComponent> attackComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> offsetShoot = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AI_Property|Component", meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> BoxCQC = nullptr;
#pragma endregion

#pragma region Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI_Property", meta = (AllowPrivateAccess))
	TEnumAsByte<EAIType> type = EAIType::Base;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateArena, Category = "AI_Property", meta = (AllowPrivateAccess))
	TObjectPtr<AArena> arena = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "AI_Property")
	TEnumAsByte<ERangeAttack> rangeAI = ERangeAttack::Mid;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "AI_Property")
	int loadingToken = 0;

	UPROPERTY(EditAnywhere, Category = "AI_Property")
	TObjectPtr<UAIsSettingsDataAsset> settings = nullptr;
	UPROPERTY(EditAnywhere, Replicated, Category = "AI_Property", meta = (UMin = 0, ClampMin = 0))
	float blendFactor = 1.f;
	UPROPERTY(EditAnywhere, Category = "AI_Property")
	bool disableAllPerception = false;
	UPROPERTY(EditAnywhere, Category = "AI_Property", meta = (UMin = 100, ClampMin = 100))
	int randomMoveRadius = 1500;
	UPROPERTY(EditAnywhere, Category = "AI_Property", meta = (UMin = 0.1f, ClampMin = 0.1f))
	float ragdollDuration = 2.f;
	UPROPERTY(EditAnywhere, Category = "AI_Property", meta = (UMin = 1.f, ClampMin = 1.f))
	float lifeSpan = 3.f;
	UPROPERTY(EditAnywhere, Category = "AI_Property")
	bool drawDebug = false;

	/// State
	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State")
	bool isEnable = false;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsFighting, Category = "AI_Property|State")
	bool isFighting = false;
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_IsDead, Category = "AI_Property|State")
	bool isDead = false;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "AI_Property|State")
	bool isSet = false;
	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State")
	bool isSpawned = false;
	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State", meta = (UMin = 0, ClampMin = 0))
	float slowedDelay = 0.5f;
	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State", meta = (UMin = 0, ClampMin = 0, UMax = 100, ClampMax = 100))
	float maxSlow = 50.f;
	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State", meta = (UMin = 0, ClampMin = 0))
	float maxDamageSlow = 50.f;
	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State")
	FStatusParams slowedStatus = FStatusParams(EStatusType::SLOW, 100, 0, EStatusType::DEFAULT_STATUS, ESpeedModifiers::NOMODIFIERS, false);

	UPROPERTY(VisibleAnywhere, Category = "AI_Property|State")
	FVector startLocation = FVector::ZeroVector;
	TObjectPtr<USkeletalMeshComponent> skeletalMesh = nullptr;

	float rotationSpeed = 1.f;
	bool isInitializationGood = false;
#pragma endregion
#pragma endregion

#pragma region Methods
public:
	AAI_Base() = default;
	AAI_Base(TEnumAsByte<EAIType> _type);

#pragma region Getter
	/// Component
	FORCEINLINE TObjectPtr<UAIStateTreeComponent> GetStateTreeComponent() const { return stateTreeComponent; }
	FORCEINLINE TObjectPtr<ULifeComponent> GetLifeComponent() const { return lifeComponent; }
	FORCEINLINE TObjectPtr<UAIBaseAnimInstance> GetAnimInstance() const { return Cast<UAIBaseAnimInstance>(GetMesh()->AnimScriptInstance); }
	/// Component/UI
	FORCEINLINE TObjectPtr<UAIDebugInfoWidgetComponent> GetDebugInfoComponent() const { return debugInfo; }
	/// Component/Perception
	FORCEINLINE TObjectPtr <UTrackingSystemComponent> GetTrackingSystemComponent() const { return trackingComponent; }
	FORCEINLINE TObjectPtr<UDamageSystemComponent> GetDamagePerceptionComponent() const { return damageComponent; }
	/// Component/Movement
	FORCEINLINE TObjectPtr<UAIMovementComponent> GetAIMovementComponent() const { return movementComponent; }
	FORCEINLINE TObjectPtr<UMovableComponent> GetMovableComponent() const { return movableComponent; }
	FORCEINLINE float GetRotationSpeed() const { return rotationSpeed; }
	/// Component/Attack
	FORCEINLINE TObjectPtr<UAttackComponent> GetAttackComponent() const { return attackComponent; }
	FORCEINLINE TObjectPtr<UShapeComponent> GetBoxCQC() const { return BoxCQC; }
	FORCEINLINE TObjectPtr<USceneComponent> GetOffsetShoot() const { return offsetShoot; }
	FORCEINLINE TObjectPtr<UWeakPointComponent> GetWeakPoint() const { return weakPoint; }
	/// Properties
	FORCEINLINE TEnumAsByte<EAIType> GetType() const { return type; }
	FORCEINLINE TObjectPtr<AArena> GetArena() const { return arena; }
	FORCEINLINE float GetBlendFactor() const { return blendFactor; }
	FORCEINLINE FVector GetStartLocation() const { return startLocation; }
	FORCEINLINE int GetRandomMoveRadius() const { return randomMoveRadius; }
	/// Properties/State
	FORCEINLINE bool IsEnable() const { return isEnable; }
	FORCEINLINE bool IsDead() const { return isDead; }
	FORCEINLINE bool IsFighting() const { return isFighting; }
	FORCEINLINE bool IsSetting() const { return isSet; }
	FORCEINLINE bool IsSpawned() const { return isSpawned; }
	FORCEINLINE bool GetDrawDebug() const { return drawDebug; }
	FORCEINLINE ERangeAttack GetRange()const { return rangeAI; }
	/// Events
	FORCEINLINE FOnEnable& OnEnable() { return onEnable; }
	FORCEINLINE FOnCombat& OnCombat() { return onCombat; }
	FORCEINLINE FOnStun& OnStun() { return onStun; }
	FORCEINLINE FOnDie& OnDie() { return onDie; }
	FORCEINLINE FOnPlayDeath& OnPlayDeath() { return onPlayDeath; }
	FORCEINLINE FOnSet& OnSet() { return onSet; }
#pragma endregion

	FORCEINLINE void SetSettings(TObjectPtr<UAIsSettingsDataAsset> _settings) { settings = _settings; }

	/// <summary>
	/// Set AI's data and component
	/// </summary>
	virtual void SetAI(AArena* _arena);
	/// <summary>
	/// Change isFighting value and update AI
	/// </summary>
	/// <param name="_value"> The new value </param>
	void SetIsFighting(const bool _value);
	FORCEINLINE void SetRange(const ERangeAttack& _range) { rangeAI = _range; }
	/// <summary>
	/// Update Tick interval
	/// </summary>
	/// <param name="_newTickInterval"> the new interval </param>
	void SetTickIntervalForActorAndComponents(float _newTickInterval);
	FORCEINLINE void SetIsSpawned(const bool _value) { isSpawned = _value; }

	/// <summary>
	/// Change visibility, collision and gravity
	/// </summary>
	/// <param name="_value"> enable or not </param>
	UFUNCTION(BlueprintCallable)
	void EnableAI(const bool _value);
	/// <summary>
	/// Active or not AI's behaviour by loadingToken
	/// </summary>
	/// <param name="_token"></param>
	void EnableBehaviour(const int _token);

	/// <summary>
	/// Update the component Tick
	/// </summary>
	/// <param name="_value"> Tick or not </param>
	void UpdateComponent(const bool _value);
	/// <summary>
	/// Swap between roam/fight mode
	/// </summary>
	void UpdateMode();
	virtual FUniqueNetIdRepl StartToFight();

	/// <summary>
	/// Stun AI for stunTime time
	/// </summary>
	/// <param name="_stunTime"> The AI's stun time </param>
	UFUNCTION()
	virtual void StunAI(const float _stunTime);
	UFUNCTION()
	virtual void SlowAI(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);

	/// <summary>
	/// Spawn carcass and play death animation
	/// </summary>
	UFUNCTION()
	void PlayDeath();
	/// <summary>
	/// Enable ragdoll on AI
	/// </summary>
	UFUNCTION()
	void Ragdoll();
	/// <summary>
	/// Destroy AI
	/// </summary>
	void InitDie();
	UFUNCTION()
	void Die();

protected:
	void BeginPlay() override;

	/// <summary>
	/// Initialize AI actor
	/// </summary>
	virtual void InitAI();

	/// <summary>
	/// The AI warn that his detect a player and get fighting mode
	/// </summary>
	/// <param name="_player"> player detected </param>
	UFUNCTION()
	void Detection();
	/// <summary>
	/// The AI lost the player and get chill mode
	/// </summary>
	UFUNCTION()
	void TargetLost();

	UFUNCTION()
	void MoveAnimation(int _axis);

#pragma region Replication
	/// <summary>
	/// Manage replication of all components
	/// </summary>
	void ComponentReplication();
	/// <summary>
	/// Replicate mode on client
	/// </summary>
	UFUNCTION()
	void OnRep_IsFighting();
	/// <summary>
	/// Replicate death animation on client
	/// </summary>
	UFUNCTION()
	void OnRep_IsDead();
	/// <summary>
	/// Replicate AI register in arena
	/// </summary>
	UFUNCTION()
	void OnRep_UpdateArena();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
#pragma endregion
#pragma endregion
};
