// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkJumpAI.generated.h"

#define NAVLINK_USE_LOG false

class AAI_Base;

UCLASS()
class ROBOTHUNTER_API ANavLinkJumpAI : public ANavLinkProxy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "NavLinkAI_Property")
	float overrideGravity = 0.f;
	UPROPERTY(EditAnywhere, Category = "NavLinkAI_Property", meta = (Umin = 0, ClampMin = 0, UMax = 1, ClmapMax = 1))
	float pushSnapPower = 0.4f;

	TObjectPtr<AAI_Base> AIOwner = nullptr;

private:
	ANavLinkJumpAI();

	void BeginPlay() override;

	UFUNCTION() 
	void StartJumpAI(AActor* _owner, const FVector& _destinationPoint);
};
