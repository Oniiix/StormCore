// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/ExponentialHeightFog.h"
#include "CustomExponentialHeightFogComponent.h"
#include "CustomExponentialHeightFog.generated.h"

/**
 * 
 */
UCLASS(showcategories = (Movement, Rendering, Transformation, DataLayers), ClassGroup = Fog)
class ROBOTHUNTER_API ACustomExponentialHeightFog : public AInfo
{
	GENERATED_BODY()
	
	UPROPERTY(Category = ExponentialHeightFog, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCustomExponentialHeightFogComponent> Component;

	UPROPERTY(ReplicatedUsing = OnRep_bEnabled)
	uint32 bEnabled : 1;

public:
	ACustomExponentialHeightFog();


private:
#if WITH_EDITOR
	virtual bool ActorTypeSupportsDataLayer() const override { return true; }
#endif

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	UFUNCTION() virtual void OnRep_bEnabled();

	virtual void PostInitializeComponents() override;

	UCustomExponentialHeightFogComponent* GetComponent() const { return Component; }
};
