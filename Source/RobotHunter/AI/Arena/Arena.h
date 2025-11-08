// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "RobotHunter/AI/Arena/Component/ArenaCombat/ArenaCombatComponent.h"
#include "NavigationInvokerComponent.h"
#include "Engine/PackageMapClient.h"
#include "RobotHunter/Game/GPE/Components/Cover/CoverComponent.h"
#include "LevelSequence.h"
#include "Arena.generated.h"

#define ARENA_USE_LOG false

class UArenaManager;
class ASpawnerAI;
class AAlienDoor;
class UTreeComponent;
class AAI_Base;
class AAI_Protector;
class AAI_Scout;
class AAI_Hunter;
class AAI_Torpedo;
class AAI_Giant;
class UAIRangePositionComponent;
class UCustomLODComponent;
class UNetworkAIComponent;
class UParalarvaSettingsDataAsset;
class UTitanSettingsDataAsset;

/// <summary>
/// Because of Multiplayer optimisation
/// </summary>
USTRUCT()
struct FAINetId
{
	GENERATED_BODY()

	FNetworkGUID NetId;

	FAINetId() = default;

	FAINetId(FNetworkGUID _NetId)
	{
		NetId = _NetId;
	}
};

/// <summary>
/// The Arena managed by ArenaManager
/// </summary>
UCLASS(HideDropdown)
class ROBOTHUNTER_API AArena : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishToCollectAI);
	FOnFinishToCollectAI onFinishToCollectAI;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParalarvaDie);
	FOnParalarvaDie onParalarvaDie;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseArena);
	UPROPERTY(BlueprintAssignable)
	FOnCloseArena onCloseArena;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClearArena, AAlienDoor*, exitGate, bool, giveBonus);
	UPROPERTY(BlueprintAssignable)
	FOnClearArena onClearArena;

#pragma region Properties
#pragma region Component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<UBillboardComponent> pin = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<UArenaCombatComponent> combatComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<USphereComponent> sphere = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<UTreeComponent> treeComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<UNavigationInvokerComponent> navInvoker = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<UAIRangePositionComponent> AIRangePositionComponent = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Arena_Property|Component", meta = (AllowprivateAccess))
	TObjectPtr<UCustomLODComponent> customLODComponent = nullptr;
#pragma endregion

#pragma region Arena
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess))
	TObjectPtr<UArenaManager> arenaManager = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess))
	bool useSequencer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess, EditCondition = "useSequencer", EditConditionHides))
	ULevelSequence* levelSequencer = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess, EditCondition = "useSequencer", EditConditionHides))
	bool hideTrapper = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess))
	FTransform arenaCameraTransform;
	UPROPERTY(VisibleAnywhere, Category = "Arena_Property|Arena")
	bool isLoaded = false;
	UPROPERTY(EditAnywhere, Replicated, Category = "Arena_Property|Arena")
	bool alwaysLoaded = false;
	UPROPERTY(EditAnywhere, Replicated, Category = "Arena_Property|Arena")
	bool disable = false;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|Arena", meta = (UMin = 100, ClampMin = 100))
	int arenaRange = 1000;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|Arena", meta = (UMin = 100, ClampMin = 100))
	int breakRange = 2000;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Arena_Property|Arena")
	int token = 0;
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Arena_Property|Arena")
	bool spawnersEnabled = false;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|Arena")
	bool drawDebug = false;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|Arena", meta = (EditCondition = "drawDebug", EditConditionHides))
	FColor debugColor = FColor::Red;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|Arena")
	FCollisionProfileName sphereCollisionProfileAI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess))
	TObjectPtr<AAlienDoor> enterGate = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Arena_Property|Arena", meta = (AllowPrivateAccess))
	TObjectPtr<AAlienDoor> exitGate = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|Arena")
	bool finalArena = false;
#pragma endregion

#pragma region AI
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA")
	TArray<TObjectPtr<ASpawnerAI>> listSpawners;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA")
	TObjectPtr<UParalarvaSettingsDataAsset> paralarvaSettings = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA", meta = (UMin = 1, ClampMin = 0, UMax = 100, ClampMax = 100))
	int maxParalarvaCount = 1;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA", meta = (UMin = 0.f, ClampMin = 0.f, UMax = 20.f, ClampMax = 20.f))
	float paralarvaCooldown = 1.f;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA")
	TObjectPtr<UTitanSettingsDataAsset> titanSettings = nullptr;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA", meta = (UMin = 1, ClampMin = 0, UMax = 100, ClampMax = 100))
	int maxTitanCount = 1;
	UPROPERTY(EditAnywhere, Category = "Arena_Property|IA", meta = (UMin = 0.f, ClampMin = 0.f, UMax = 20.f, ClampMax = 20.f))
	float titanCooldown = 1.f;
#pragma endregion

#pragma region List
	UPROPERTY()
	TMap<uint32, AAI_Base*> mapAIs;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arena_Property|List", meta = (AllowPrivateAccess))
	TArray<TObjectPtr<AAI_Base>> listAIs;
	UPROPERTY(VisibleDefaultsOnly, Category = "Arena_Property|List", Replicated)
	int countParalarvas;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Arena_Property|List", Replicated, meta = (AllowPrivateAccess)) 
	int countTrappers;
	UPROPERTY(VisibleDefaultsOnly, Category = "Arena_Property|List", Replicated)
	int countTitans;

	UPROPERTY()
	TArray<UCoverComponent*> listCover;
#pragma endregion

	int spawnersEndedCount = 0;
	int spawnerIndex = 0;
	int spawnerLastIndex = 0;
	int totalAIs = 0;
	FName defaultCollisionProfileSphere;
	TObjectPtr<UNetworkAIComponent> networkAI = nullptr;
	bool isInitializationGood = false;
#pragma endregion

#pragma region Methods
public:
	AArena();

	/// Getter
	FORCEINLINE FOnFinishToCollectAI& OnFinishToCollectAI() { return onFinishToCollectAI; }
	FORCEINLINE FOnClearArena& OnClearArena() { return onClearArena; }
	FORCEINLINE FOnParalarvaDie& OnParalarvaDie() { return onParalarvaDie; }
	FORCEINLINE int GetArenaRange() const { return arenaRange; }
	FORCEINLINE int GetBreakRange() const { return breakRange; }
	FORCEINLINE TMap<uint32, AAI_Base*> GetMapAIs() const { return mapAIs; }
	FORCEINLINE TObjectPtr<UArenaManager> GetArenaManager() const { return arenaManager; }
	FORCEINLINE TObjectPtr<UArenaCombatComponent> GetCombatComponent() const { return combatComponent; }
	FORCEINLINE TObjectPtr<UAIRangePositionComponent> GetAIRangePositionComponent() const { return AIRangePositionComponent; }
	FORCEINLINE TObjectPtr<USphereComponent> GetSphere() const { return sphere; }
	FORCEINLINE bool IsLoaded() const { return isLoaded; }
	FORCEINLINE bool IsDisable() const { return disable; }
	FORCEINLINE TArray<UCoverComponent*> GetListCover() { return listCover; }
	FORCEINLINE int GetTrackingRange() const { return breakRange + combatComponent->GetAdditionalRange(); }
	FORCEINLINE ULevelSequence* GetLevelSequencer() { return levelSequencer; }
	FORCEINLINE TObjectPtr<AAlienDoor> GetExitGate() { return exitGate; }
	FORCEINLINE TObjectPtr<UParalarvaSettingsDataAsset> GetParalarvaSettings() { return paralarvaSettings; }
	FORCEINLINE TObjectPtr<UTitanSettingsDataAsset> GetTitanSettings() { return titanSettings; }
	UFUNCTION(BLueprintPure) FORCEINLINE bool GetFinalArena() const { return finalArena; }
	/// Setter
	FORCEINLINE void SetAlwaysLoaded(const bool _value) { alwaysLoaded = _value; }
	FORCEINLINE void SetSpawnersEnabled(const bool _value) { spawnersEnabled = _value; }

	// Loading
	/// <summary>
	/// Display/Hide all actors in arena and enable/disable all AIs
	/// </summary>
	/// <param name="_loaded"> true if loaded, false instead </param>
	UFUNCTION()
	void Load(const bool _loaded);
	UFUNCTION()
	void Loading();
	/// <summary>
	/// Handle multi loading process
	/// </summary>
	/// <param name="_isLoaded"> loaded or not </param>
	void LoadManagement(const bool _isLoaded);

	// Spawning
	/// <summary>
	/// Add the given AI to relevant list
	/// </summary>
	/// <param name="_newAI"> the new AI to add </param>
	void AddAI(AAI_Base* _newAI);

	void RegisterInAILists(AAI_Base* _newAI);
	void RemoveAIFromLists(AAI_Base* _AI);

	void RemoveUseless();

	/// <summary>
	/// Spawn a number of AI on random location
	/// </summary>
	/// <param name="_ref"> the index of listAIsInitial to catch class ref </param>
	/// <param name="_count"> the number of AIs to spawn </param>
	void SpawnAIsAtRandomLocation(const TSubclassOf<AAI_Base>& _ref, const int _count);

	/// <summary>
	/// Get a random location in Arena, avoid close perimeter of nest
	/// </summary>
	/// <returns> a location in Arena </returns>
	FVector RandomLocationInArena();

	UFUNCTION(BlueprintCallable)
	void EndSequencer();
	void ClearArena(const bool _giveBonus);
	void CloseArena();
	UFUNCTION(BlueprintCallable)
	void SetupPlayerCamera();
	void FillMissingAIs();

private:
	// Start
	void BeginPlay() override;
	/// <summary>
	/// Initialize the Arena
	/// </summary>
	void InitArena();
	/// <summary>
	/// Get all AIs in arena range using sphere
	/// </summary>
	UFUNCTION()
	void CollectAIsInRange();
	/// <summary>
	/// Register Arena in SpawnManager
	/// </summary>
	bool RegisterArena();
	UFUNCTION()
	virtual void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Tick(float DeltaTime) override;
	bool ShouldTickIfViewportsOnly() const;

	UFUNCTION()
	void StartCombat(FUniqueNetIdRepl _playerNetID);

	// AIs
	/// <summary>
	/// Handle AI's death in Arena
	/// </summary>
	/// <param name="_AI"> AI that want to die </param>
	UFUNCTION()
	void CheckAI(AAI_Base* _AI);
	/// <summary>
	/// Delay enable/disable AI to save performance
	/// </summary>
	/// <param name="_AI"> AI to enable/disable </param>
	UFUNCTION()
	void DelayedEnableAI(AAI_Base* _AI);
	/// <summary>
	/// Delay the spawn of AI to save performance
	/// </summary>
	/// <param name="_ref"> Ref of AI to spawn </param>
	UFUNCTION()
	void DelayedSpawnAI(const TSubclassOf<AAI_Base>& _ref);
	void DispatchAIBetweenSpawner(AAI_Base* _AI);

	// Utils
	/// <summary>
	/// Draw the debug of Arena
	/// </summary>
	void DrawDebug();

	// Replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion 
};
