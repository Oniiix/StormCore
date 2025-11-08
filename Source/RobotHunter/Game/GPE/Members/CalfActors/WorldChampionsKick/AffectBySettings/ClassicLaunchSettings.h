// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LaunchSettings.h"
#include "ClassicLaunchSettings.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UClassicLaunchSettings : public ULaunchSettings
{
	GENERATED_BODY()

	TObjectPtr<AActor> owner = nullptr;
	TObjectPtr<UStaticMeshComponent> currentMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool isOnFloor = true;
	UPROPERTY(VisibleAnywhere)
	bool physicsStateBeforeAffect = false;

public:
	virtual void Init(AActor* _owner) override;

	virtual void Launch(const FVector& _dir) override;

private:
	UFUNCTION() virtual void Touch(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION() virtual void ActivateHitEvent();

	TObjectPtr<UStaticMeshComponent> TryGetMesh();
};
