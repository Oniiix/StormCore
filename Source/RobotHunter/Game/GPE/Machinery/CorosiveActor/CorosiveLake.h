// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "CorosiveLake.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ACorosiveLake : public AReceiver
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> acidMesh = nullptr;


protected:
	UPROPERTY(EditAnywhere, Category = "Custom|CorosiveLake")
	float maxHeight = 3;
	UPROPERTY(EditAnywhere, Category = "Custom|CorosiveLake")
	float damage = 10;
	UPROPERTY(EditAnywhere, Category = "Custom|CorosiveLake")
	float growSpeed = 50;
	UPROPERTY(EditAnywhere, Category = "Custom|CorosiveLake")
	float reduceSpeed = 50;
	UPROPERTY(EditAnywhere, Category = "Custom|CorosiveLake")
	float damageTime = 0.2;

	TObjectPtr<AMainCharacter> player = nullptr;

	FVector initScale, maxScale;

	FTimerHandle timerDamage;
	
public:
	ACorosiveLake();

	virtual void Action() override;

protected:
	virtual void Tick(float _deltaTime)override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;
	virtual void NotifyActorEndOverlap(AActor* _otherActor)override;

	virtual void InitReceiver() override;
private:
	void GrowAcid();
	void ReduceAcid();
	void DamagePlayer();
};
