// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RobotHunter/AI/Component/Widget/LifebarWidgetComponent.h"
#include "RobotHunter/AI/Component/Movement/AIMovementComponent.h"
#include "CamouflageComponent.generated.h"

#define CAMOUFLAGE_COMPONENT_USE_LOG false

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROBOTHUNTER_API UCamouflageComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTransform);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnTransform onTransform;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableTransform);
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnDisableTransform onDisableTransform;

	UPROPERTY(EditAnywhere, Category = "Custom Property")
	FStatusParams protectionStatus;

	UPROPERTY(EditAnywhere, Category = "Custom Property")
	FStatusParams stunResistantStatus;

	UPROPERTY(EditAnywhere, Category = "Custom Property")
	FStatusParams staggerResistantStatus;

	UPROPERTY()
	TObjectPtr<ULifebarWidgetComponent> lifeBar = nullptr;

	UPROPERTY()
	TObjectPtr<UStatusComponent> statusComp = nullptr;

	TObjectPtr<UAIMovementComponent> movementComponent = nullptr;

	float speedCamouflage = 0;

public:	
	UCamouflageComponent();

	FORCEINLINE FOnTransform& OnTransform()
	{
		return onTransform;
	}

	FORCEINLINE void SetSpeedCamouflage(const float _speedCamouflage)
	{
		speedCamouflage = _speedCamouflage;
	}
protected:
	virtual void BeginPlay() override;

public:
	/// <summary>
	/// Transform ai in random mesh
	/// </summary>
	void TransformMode();

	/// <summary>
	/// Disable camouflage mesh
	/// </summary>
	void DisableMesh();

	void UpdateCamouflageStatusValue(const EStatusType& _type, const float _value);
};
