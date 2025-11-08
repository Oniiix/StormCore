// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/Interface/PerceptionInterface/PerceptionInterface.h"

#include "PerceptionSystemGPEComponent.generated.h"


UCLASS(Abstract)
class ROBOTHUNTER_API UPerceptionSystemGPEComponent : public UActorComponent, public IPerceptionInterface
{
	GENERATED_BODY()
protected:
#pragma region Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetected, AActor*, _player);
	FOnDetected onDetected;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLost);
	FOnLost onLost;
#pragma endregion
protected:
	UPROPERTY(EditAnywhere, Category = "Perception|Property")float rangePerception = 100;
	UPROPERTY(EditAnywhere, Category = "Perception|Property") FVector perceptionLocation;

	UPROPERTY(EditAnywhere, Category = "Perception|Property")TArray<TEnumAsByte<EObjectTypeQuery>> layersDetection = {};
	UPROPERTY(EditAnywhere, Category = "Perception|Property")bool disablePerception = false;
	UPROPERTY(VisibleAnywhere, Category = "Perception|Property") TObjectPtr<AActor> target = nullptr;
	bool hasTarget = false;

public:
	FORCEINLINE FOnDetected& OnDetected() { return onDetected; }
public:
	UPerceptionSystemGPEComponent();
public:
	FORCEINLINE FVector PerceptionOffLocation() const { return GetOwner()->GetActorLocation() + perceptionLocation; }
	FORCEINLINE AActor* GetTarget() const { return target; }
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void PerceptionBehaviour() override {};
	virtual	void Detected(AActor* _actorDetected) override;
	virtual void ClearTarget() override;
	virtual void DrawDebug() {}
public:
	void ActivatePerception();
	void DesactivatePerception();
};
