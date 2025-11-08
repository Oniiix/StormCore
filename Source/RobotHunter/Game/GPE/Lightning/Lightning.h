// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/CustomActor.h"
#include "Lightning.generated.h"


//TODO access Stun by Component
class AMainCharacter;
class AAI_Base;
class AStormFlower;
class USphereComponent;

class AStormFlowerV2;
//TODO damage and stun player/AI

#define LIGHTNING_USE_LOG false

UCLASS()
class ROBOTHUNTER_API ALightning : public ACustomActor
{
	GENERATED_BODY()
private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE (FOnCall);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE (FOnLightningStrike); 
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events",meta = (AllowPrivateAccess))
	FOnCall onCall;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events", meta = (AllowPrivateAccess))
	FOnLightningStrike onLightningStrike;
private:
	UPROPERTY(EditAnywhere, Category = "Property") TArray<TEnumAsByte<EObjectTypeQuery>> layersObstacle = {};
	UPROPERTY(EditAnywhere, Category = "Property") TArray<TEnumAsByte<EObjectTypeQuery>> layersDetection = {};
	UPROPERTY(EditAnywhere, Category = "Property", meta = (ClampMin = 100)) float radiusDamage = 500;
	UPROPERTY(EditAnywhere, Category = "Property", meta = (ClampMin = 1)) float previewTime = 3;
	UPROPERTY(EditAnywhere, Category = "Property", meta = (ClampMin = 1)) float timeStun = 5;
	UPROPERTY(EditAnywhere, Category = "Property", meta = (ClampMin = 1)) float damage = 1;
	UPROPERTY(EditAnywhere, Category = "Property") TObjectPtr< USphereComponent> sphere = nullptr;
	UPROPERTY(Replicated)  float speedLightning;

	UPROPERTY()
	TObjectPtr< AStormFlower> stormflowerTouching = nullptr;

	UPROPERTY()
	TObjectPtr<AStormFlowerV2> touchedStormFlower = nullptr;


	FTimerHandle timerPreviewTime;
	TArray<AActor*> actorTouchingByLight;
public:
	FORCEINLINE FOnCall& OnCall() { return onCall; }
	FORCEINLINE FOnLightningStrike& OnLightningStrike(){ return onLightningStrike; }
public:
	ALightning();
public:
	UFUNCTION(BlueprintCallable) float GetRadiusDamage()const { return radiusDamage; }
	UFUNCTION(BlueprintCallable) float GetPreviewTime()const { return previewTime; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;
private:
	void GeneratePreview();
	void CheckCeiling();
	void GenerateLightning();
	UFUNCTION()void GenerateDamageZone();

	void CheckStormFlower();
	void LookForStormFlowerV2();

	void Destroy();
	virtual void DrawDebug() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION() void BeginOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* 
		_otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _weepResult);


	UFUNCTION() void EndOverlap(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, 
		int32 _otherBodyIndex);
};