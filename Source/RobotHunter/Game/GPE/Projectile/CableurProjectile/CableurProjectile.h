// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "../../../../CustomActor.h"
#include <NiagaraSystem.h>
#include <NiagaraComponent.h>
#include <Components/BoxComponent.h>
#include "CableurNail.h"
#include "CableurProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ACableurProjectile : public ACustomActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSetupCable, FVector, _start, FVector, _end);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess))
	TObjectPtr<UNiagaraSystem> cableNiagara;
	USceneComponent* cableNiagaraSceneComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess))
	UNiagaraComponent* cableNiagaraComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess))
	float platformLifetime = 5;
	UPROPERTY(EditAnywhere)
	float launchingForce = 1000;

	FVector startLocation;
	FVector endLocation;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnSetupCable onSetupCable;

	UPROPERTY(EditAnywhere);
	bool useHitBox = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> hitBox;

	UPROPERTY()
	TObjectPtr<ACableurNail> nail1 = nullptr;
	TObjectPtr<ACableurNail> nail2 = nullptr;

public:

	FORCEINLINE void SetStartLocation(FVector _location) { startLocation = _location; }
	FORCEINLINE void SetEndLocation(FVector _location) { endLocation = _location; }
	FORCEINLINE void SetNail1(ACableurNail* _nail1) { nail1 = _nail1; }
	FORCEINLINE void SetNail2(ACableurNail* _nail2) { nail2 = _nail2; }
	FORCEINLINE FOnSetupCable& OnSetupCable() { return onSetupCable; }


public:
	ACableurProjectile();
	void BeginPlay();
	void Init();
	UFUNCTION() void Bounce(UPrimitiveComponent* _OverlappedComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	void SpawnNiagara();
};
