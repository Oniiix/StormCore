// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaunchSettings.h"
#include "AffectByWCKComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UAffectByWCKComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float weight = 1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULaunchSettings> wckSettings = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULaunchSettings> currentWCKSettings = nullptr;

public:	
	UAffectByWCKComponent();

protected:
	virtual void BeginPlay() override;	

public:
	virtual void Affect(const FVector& _dir);
};
