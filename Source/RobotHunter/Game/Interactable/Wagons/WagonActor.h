#pragma once

#include "CoreMinimal.h"
#include "../InteractableActor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "../../../Splines/Rail/RailActor.h"
#include "WagonActor.generated.h"

#define WAGON_USE_LOG false

UCLASS()
class ROBOTHUNTER_API AWagonActor : public AInteractableActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Rail")
	TObjectPtr<ARailActor> currentRail;

	UPROPERTY(EditAnywhere, Category = "Rail|Detection")
	float detectionRange;

	UPROPERTY(EditAnywhere, Category = "Rail|Detection")
	TObjectPtr<USphereComponent> detectionSphere;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float movementSpeed = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float rotationSpeed = 25.0f;

	UPROPERTY(VisibleAnywhere)
	bool isBlocked;

	UPROPERTY(VisibleAnywhere/*, ReplicatedUsing = OnRep_UpdateIsStopped*/)
	bool isStopped;

	UPROPERTY(VisibleAnywhere/*, ReplicatedUsing = OnRep_UpdateIsOnReturn*/)
	bool isOnReturn;

	float currentDistance = 0.0f;
	bool changeWayBefore;
	bool changeWayAfter;
	bool inputsSetup;

	UPROPERTY()
	TArray<ARailActor*> detectedRails;

public:
	AWagonActor();

	FORCEINLINE void SetIsStopped(const bool _isStopped) { isStopped = _isStopped; }
	FORCEINLINE void SetIsOnReturn(const bool _isOnReturn) { isOnReturn = _isOnReturn; }

	FORCEINLINE ARailActor* GetCurrentRail() const { return currentRail; }
	FORCEINLINE TArray<ARailActor*> GetDetectedRails() const { return detectedRails; }

private:
	void UpdateDetectionSphere();

	UFUNCTION() void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION() void RefreshDetectedRails();
	bool CheckIfRailIsOnPath(ARailActor* _rail, const bool _remove = true);
	//bool CheckIfLeverContainsRail(ALeverActor* _lever, ARailActor* _rail);
	bool CheckIfTargetContainsRail(ARailsTargetActor* _target, ARailActor* _rail);

protected:
	virtual void Move(float DeltaTime);
	virtual void Rotate(float DeltaTime);
	void UpdateCurrentRail();

	void Stop();

#pragma region Network
	//UFUNCTION() void OnRep_UpdateIsBlocked(const bool _isBlocked);
	/*UFUNCTION() void OnRep_UpdateIsStopped();
	UFUNCTION(Server, UnReliable) void ServerRpc_SetIsStopped(const bool _isStopped);
	UFUNCTION() void OnRep_UpdateIsOnReturn();
	UFUNCTION(Server, UnReliable) void ServerRpc_SetIsOnReturn(const bool _isOnReturn);*/
	//UFUNCTION() void OnRep_UpdateCurrentDistance(const float _currentDistance);
	//UFUNCTION() void OnRep_UpdateCurrentRail(ARHRailActor* _rail);
	// 
	//UFUNCTION(Server, Unreliable) void ServerRPC_Interacted();
#pragma endregion Network

	void SetupPlayerInputs(AMainCharacter* _player);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UFUNCTION(CallInEditor, Category = "Interaction") virtual void Interacted(AMainCharacter* _character) override;

	void ChangeWay();
};
