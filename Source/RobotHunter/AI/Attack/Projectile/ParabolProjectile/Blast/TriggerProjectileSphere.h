// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "TriggerProjectileSphere.generated.h"

UENUM()
enum ETypeBlast
{
	INSTANT,
	DURING,
};

UCLASS()
class ROBOTHUNTER_API ATriggerProjectileSphere : public ATriggerSphere
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInit, const float, _radius);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowprivateAccess))
	FOnInit onInit;

	UPROPERTY(EditAnywhere, Category = "Custom|Property", BlueprintReadWrite, meta = (AllowprivateAccess))
	float lifeSpanTrigger = 5;
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float damage = 0;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	TEnumAsByte< ETypeBlast> typeBlast;
	UPROPERTY(EditAnywhere, Category = "Custom|Property|During", meta = (EditCondition = "typeBlast == ETypeBlast::DURING", EditConditionHides))
	float timeDamage = 0;

	UPROPERTY(EditAnywhere, Category = "Custom|Property", BlueprintReadWrite, meta = (AllowprivateAccess))
	FHitResult hitResult;

	UPROPERTY()
	TObjectPtr<AMainCharacter> player = nullptr;

	FTimerHandle timerDamage, timerInit;

public:
#pragma region Setteur
	FORCEINLINE void SetDamage(const float _damage)
	{
		damage = _damage;
	}
	FORCEINLINE void SetTimeDamage(const float _timeDamage)
	{
		timeDamage = _timeDamage;
	}
#pragma endregion

	void InitTrigger(const TArray< TEnumAsByte<ECollisionChannel>>& _collision, const float _radius, const FHitResult& _result);

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor)override;

	void TypeDamage();
	void Damage();

	UFUNCTION() void PlayerDead();

public:
	UFUNCTION() void ClearArena(AAlienDoor* _exitGate, bool _giveBonus);
};
