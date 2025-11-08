// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "NiagaraComponent.h"
#include "DroneFireflies.generated.h"

/**
 * 
 */
class AFirefliesActor;

UCLASS()
class ROBOTHUNTER_API ADroneFireflies : public ACustomActor
{
	GENERATED_BODY()

#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDesactiveMark, AActor*, _target, ADroneFireflies*,_drone);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnDesactiveMark onDisableMark;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMarkConfirmed, AActor*, _target, ADroneFireflies*,_drone);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnMarkConfirmed onMarkConfirmed;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOutView, AActor*, _target);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnOutView onOutView;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInView, AActor*, _target);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnInView onInView;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInit);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnInit onInit;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProgress, float, _markProgress);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnProgress onProgress;
#pragma endregion

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float speedDestroyDrone = 10;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float timeDisableDrone = 0;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float timeMark = 0;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	float elapsedTimeMark = 0;

	UPROPERTY()
	TObjectPtr<AActor> target = nullptr;
	UPROPERTY()
	TObjectPtr<AMainCharacter> player = nullptr;
	UPROPERTY()
	TObjectPtr<AFirefliesActor> arm = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMesh> targetMesh = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMesh> targetSkeletal = nullptr;

	TObjectPtr<UNiagaraComponent> niagara = nullptr;

	FVector newLocation;
	FTimerHandle timerDisableMark, timerMark;

	TObjectPtr<UCameraComponent> cam = nullptr;

	bool disableDefMark = false;
	bool isOutView = false;
	bool isCurrentMarkTimer = false;
	bool stopMark = false;

	float timeElapsed = 0;

protected:
	virtual void BeginPlay()override;
	virtual void Tick(float _deltaTime)override;

private:
	void IsOutView(const float _angle);
	void IsInView(const float _angle);

public:
#pragma region EventMethod
	FORCEINLINE FOnDesactiveMark& OnDisableMark()
	{
		return onDisableMark;
	}
	FORCEINLINE FOnMarkConfirmed& OnMarkConfirmed()
	{
		return onMarkConfirmed;
	}
	FORCEINLINE FOnOutView& OnOutView()
	{
		return onOutView;
	}
	FORCEINLINE FOnInView& OnInView()
	{
		return onInView;
	}
#pragma endregion

	FORCEINLINE AActor* GetEnemy()const
	{
		return target;
	}

	void InitDrone(AActor* _target, const float _timeMark, const float _timeDesactiveDrone, AFirefliesActor* _arm);
	void StartMark();
	UFUNCTION() void DisableMark();
	UFUNCTION() void DisableDefMark();

	void StopMark();
	void DestroyDrone();
};
