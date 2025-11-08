#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/Interactable/MoveInteractable.h"
#include "RobotHunter/Game/GPE/Transmitter/TargetActor/RailsTargetActor.h"
#include "Components/BoxComponent.h"
#include "StopHandcarActor.generated.h"

class UStopHandcarManager;

#define STOP_HANDCAR_DEBUG false


UCLASS()
class ROBOTHUNTER_API AStopHandcarActor : public AMoveInteractable
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandcarStop, bool, isStopped);

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnHandcarStop onHandcarStop;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandcarOverlap, bool, isOverlapping);

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnHandcarOverlap onHandcarOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> buttonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> playerPosition; 
	
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (UMin = 1.0f, ClampMin = 1.0f, UMax = 360.0f, ClampMax = 360.0f))
	float interactableAngle = 180.0f;

	UPROPERTY(EditAnywhere, Category = "StopZone")
	TObjectPtr<UBoxComponent> stopZone;

	UPROPERTY(EditAnywhere, Category = "StopZone")
	TObjectPtr<UBoxComponent> disembarkZone;

	UPROPERTY(EditAnywhere, Category = "AutoMode")
	TMap<AStopHandcarActor*, TEnumAsByte<ERailWay>> autoModeWays;

	UPROPERTY()
	TObjectPtr<UStopHandcarManager> manager;

	UPROPERTY()
	TObjectPtr<ACustomHandcarActor> handcar;

	UPROPERTY()
	TObjectPtr<UNetworkGPEComponent> networkComp;

	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<AMainCharacter> player;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateCalledHandcar)
	bool calledHandcar;

	//bool movePlayer;
	//bool rotatePlayer;

public:
	FORCEINLINE void SetCalledHandcar(const bool _calledHandcar) { calledHandcar = _calledHandcar; }
	FORCEINLINE ACustomHandcarActor* GetHandcar() const { return handcar; }
	FORCEINLINE TEnumAsByte<ERailWay> GetNextRailWay(AStopHandcarActor* _nextStop)
	{
		if (autoModeWays.IsEmpty() || !_nextStop || !autoModeWays.Contains(_nextStop))
			return ERailWay::DefaultRailWay;

		return autoModeWays[_nextStop];
	}

	FORCEINLINE FOnHandcarStop& GetOnHandcarStop() { return onHandcarStop; }
	FORCEINLINE FOnHandcarOverlap& GetOnHandcarOverlap() { return onHandcarOverlap; }

public:
	AStopHandcarActor();

private:
	void Init();
	void MovePlayerToPosition();
	void RotatePlayerTowardsPosition();
	void CallHandcar();

	UFUNCTION() void OnStopZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnStopZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() void OnDisembarkZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnDisembarkZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interacted(AMainCharacter* _character) override;
	virtual void LaunchAnimation(AMainCharacter* _character) override;
	virtual bool CanInteract(AMainCharacter* _character) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void Interacted_Internal() override;

public:
	UFUNCTION() void OnRep_UpdateCalledHandcar(const bool _called);
};
