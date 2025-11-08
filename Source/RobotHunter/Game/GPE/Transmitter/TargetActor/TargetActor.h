// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "RobotHunter/AI/Component/Life/LifeComponent.h"
#include "RobotHunter/Game/GPE/Machinery/Receiver.h"
#include "../TransmitterActor.h"
#include "TargetActor.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API ATargetActor : public ATransmitterActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULifeComponent> lifeComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Custom|Target")
	float cooldownTime = 2;

	bool hasShoot = false;

public:
	ATargetActor();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION() virtual void ActivateTarget(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);
};
