// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Components/BoxComponent.h>
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Attack/Projectile/LineProjectile/LineProjectile.h"
#include "RobotHunter/AI/Attack/Projectile/ProjectileStat/LineProjectileStat/TongueProjectileStat/TongueProjectileStat.h"
#include "TongueProjectile.generated.h"

#define TONGUE_PROJECTILE_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API ATongueProjectile : public ALineProjectile
{
	GENERATED_BODY()

	TObjectPtr< UTongueProjectileStat> tongueStat = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Projectile|Tongue")
	TObjectPtr<UBoxComponent> triggerBox = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Projectile|Tongue")
	TArray<TEnumAsByte<EObjectTypeQuery>> outLayer;

	UPROPERTY()
	float speed;

	TObjectPtr<AMainCharacter> player;
	FVector newScale, initScale, maxScale;
	FVector newLocation;
	FTimerHandle timerReturn;
	bool returnMode = false;

public:
	ATongueProjectile();

	FORCEINLINE UTongueProjectileStat* GetTongueStat() const 
	{
		return tongueStat; 
	}
	FORCEINLINE UBoxComponent* GetTriggerBox() const 
	{
		return triggerBox;
	}

#pragma region MoveSize
	void SizeTongue();
	void ReturnMode();
#pragma endregion

	virtual void InitProjectile(const FVector& _targetLocation) override;

protected:
	virtual void Tick(float _deltaTime) override;

	UFUNCTION() void TriggerCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
