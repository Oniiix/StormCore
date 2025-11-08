// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/AI/Arena/ArenaManager.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "ArenaCombatComponent.generated.h"

#define COMBAT_COMPONENT_USE_LOG false

class AArena;
class AAI_Base;
class AMainCharacter;
class UAIsAttackDataAsset;
struct FAICQCAttack;
struct FAIDistanceAttack;

USTRUCT()
struct FAIsOnPlayer
{
	GENERATED_BODY();

public:
	int paralarvaCount = 0;
	int trapperCount = 0;
	int titanCount = 0;
	//int pennantCount = 0;
	//int torpedoCount = 0;

	FAIsOnPlayer() = default;

	FORCEINLINE void Update(const EAIType& _AIType, const bool _add)
	{
		switch (_AIType)
		{
		case Paralarva:
			_add ? paralarvaCount++ : paralarvaCount--;
			CLAMP(paralarvaCount, 0, INT_MAX);
			break;
		case Trapper:
			_add ? trapperCount++ : trapperCount--;
			CLAMP(trapperCount, 0, INT_MAX);
			break;
		case Titan:
			_add ? titanCount++ : titanCount--;
			CLAMP(titanCount, 0, INT_MAX);
			break;
		//case Pennant:
		//	_add ? pennantCount++ : pennantCount--;
		//	CLAMP(pennantCount, 0, INT_MAX);
		//	break;
		//case Torpedo:
		//	_add ? torpedoCount++ : torpedoCount--;
		//	CLAMP(torpedoCount, 0, INT_MAX);
		//	break;
		default:
			break;
		}
	}
	FORCEINLINE TArray<int> GetAIsCount() { return { paralarvaCount, trapperCount, titanCount }; }
	FORCEINLINE void Empty() { paralarvaCount = 0; trapperCount = 0; titanCount = 0; }
};

/**
 * The component that handle arena's combat
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UArenaCombatComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishInitialization);
	FOnFinishInitialization onFinishInitialization;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertArena, bool, _onAlert);
	FOnAlertArena onAlertArena;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnterArena, FUniqueNetIdRepl, _playerNetID);
	FOnPlayerEnterArena onPlayerEnterArena;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitArena, FUniqueNetIdRepl, _playerNetID);
	FOnPlayerExitArena onPlayerExitArena;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAIsOnPlayer, TArray<FUniqueNetIdRepl>, _listPlayer, TArray<FAIsOnPlayer>, _listAIsOnPlayer);
	FOnUpdateAIsOnPlayer onUpdateAIsOnPlayer;
#pragma endregion

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "ArenaCombat_Property")
	TObjectPtr<UAIsAttackDataAsset> AIsAttack = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "ArenaCombat_Property")
	TObjectPtr<UAIsAttackDataAsset> AIsAttackInstance = nullptr;
	UPROPERTY(EditAnywhere, Category = "ArenaCombat_Property", meta = (UMin = 0, ClampMin = 0))
	int additionalRangeForTrackingPlayer = 500;
	UPROPERTY(EditAnywhere, Category = "ArenaCombat_Property")
	bool drawDebug = false;
	UPROPERTY(EditAnywhere, Category = "ArenaCombat_Property", meta = (EditCondition = "drawDebug", EditConditionHides))
	FColor debugColor = FColor::Black;
	UPROPERTY(EditAnywhere, Category = "ArenaCombat_Property")
	FCollisionProfileName sphereCollisionProfilePlayer;

	// List & map
	UPROPERTY(VisibleAnywhere, Category = "ArenaCombat_Property")
	TArray<FUniqueNetIdRepl> listPlayersId;
	UPROPERTY(VisibleAnywhere, Category = "ArenaCombat_Property")
	TMap<FUniqueNetIdRepl, FAIsOnPlayer> mapAIsOnPlayer;

	UPROPERTY(VisibleAnywhere, Category = "ArenaCombat_Property")
	bool onAlert = false;

	TObjectPtr<USphereComponent> trackingSphere = nullptr;
	TArray<AMainCharacter*> listPlayerInRange;
	int trackingRange;

	FTimerHandle timerCustomTick;
	TObjectPtr<AArena> owner = nullptr;
	bool isInitializationGood = false;
#pragma endregion

#pragma region Methods
public:
	UArenaCombatComponent();

	FORCEINLINE FOnUpdateAIsOnPlayer& OnUpdateAIsOnPlayer() { return onUpdateAIsOnPlayer; }
	FORCEINLINE FOnFinishInitialization& OnFinishInitialization() { return onFinishInitialization; }
	FORCEINLINE FOnAlertArena& OnAlertArena() { return onAlertArena; }
	FORCEINLINE FOnPlayerEnterArena& OnPlayerEnterArena() { return onPlayerEnterArena; }
	FORCEINLINE FOnPlayerExitArena& OnPlayerExitArena() { return onPlayerExitArena; }
	FORCEINLINE int GetAdditionalRange() const { return additionalRangeForTrackingPlayer; }
	FORCEINLINE TMap<FUniqueNetIdRepl, FAIsOnPlayer> GetMapAIsOnPlayer() { return mapAIsOnPlayer; }
	FORCEINLINE bool IsOnAlert() const { return onAlert; }
	FORCEINLINE TArray<AMainCharacter*> GetListPlayerInRange() const { return listPlayerInRange; }

	/// <summary>
	/// Initialize component
	/// </summary>
	void InitArenaCombat();

	void GetShortAttacksForAI(const EAIType& _AIType, TArray<FAICQCAttack>& _listShortAttack, int& _indexCQCAttack);
	TArray<FAIDistanceAttack> GetDistanceAttacksForAI(const EAIType& _AIType, const ERangeAttack& _range);
	void GetTokenOnCQCAttack(const int _index, const FAICQCAttack& _CQCData);
	void GetTokenOnDistanceAttack(const ERangeAttack& _range, const int _index, const FAIDistanceAttack& _DistanceData);
	void StartGenerateAttackTokenTimer(const ERangeAttack& _range, const int _index, const float _delay);

	/// <summary>
	/// Triggered alert on arena, alerting all AIs
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ArenaAlert();
	/// <summary>
	/// Alert AI to fight
	/// </summary>
	/// <param name="_AI"> the AI to alert </param>
	void AlertAI(AAI_Base* _AI);

	UFUNCTION()
	void OnAIChangeTarget(FUniqueNetIdRepl _oldTargetId, FUniqueNetIdRepl _newTargetId, AAI_Base* _AI);
	/// <summary>
	/// Handle the death of AI
	/// </summary>
	/// <param name="_AI"> The dying AI </param>
	UFUNCTION()
	void AIDie(AAI_Base* _AI);

	AMainCharacter* GetCharacterByNetId(FUniqueNetIdRepl _id);

private:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool ShouldTickIfViewportsOnly() const;

	UFUNCTION()
	void SetListPlayers();

	void UpdateAIsOnPlayer(FUniqueNetIdRepl _playerId, AAI_Base* _AI, const bool _add);

	UFUNCTION()
	void SetTrackingSphere();
	UFUNCTION()
	void TrackingSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void TrackingSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// Draw debug of combatComponent
	/// </summary>
	void DrawDebug();
#pragma endregion
};
