// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/Attack/Projectile/ParabolProjectile/Blast/BlastProjectile.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "ScoutProjectile.generated.h"

UCLASS(HideDropdown)
class ROBOTHUNTER_API AScoutProjectile : public ABlastProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Blast")
	float timeDamageBlast = 1;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|Blast")
	float timeDuringBlast = 3;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Blast")
	TArray<TEnumAsByte<EObjectTypeQuery>> outLayer;
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

	void InitBlast(const AMainCharacter* _chara);
	void BlastProjectile(const FVector& _origin);
};
