// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/AI/Component/Camouflage/CamouflageComponent.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "RobotHunter/Game/GPE/Components/Cover/CoverComponent.h"
#include "AI_Hunter.generated.h"

#define AI_TRAPPER_USE_LOG false

UCLASS(HideDropdown)
class ROBOTHUNTER_API AAI_Hunter : public AAI_Base
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartCamouflage);
	FOnStartCamouflage onStartCamouflage;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopCamouflage);
	FOnStopCamouflage onStopCamouflage;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartDisableCamouflage);
	FOnStartDisableCamouflage onStartDisableCamouflage;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopDisableCamouflage);
	FOnStopDisableCamouflage onStopDisableCamouflage;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootTongue);
	FOnShootTongue onShootTongue;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCritique);
	FOnCritique onCritique;

	UPROPERTY(EditAnywhere, Category = "Trapper_Component")
	TObjectPtr<UCamouflageComponent> camouflageComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float outRangeRandomMove = 600;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float offSetShootForLine = 200;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Camouflage")
	float speedCamouflage = 500;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Dodge")
	float cooldownDodge = 2;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Dodge")
	float damageDodge = 30;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Dodge")
	float damageTimeDodge = 3;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Dodge", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float lengthDodge = 300;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Dodge", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float speedDodge = 500;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Dodge", BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<TEnumAsByte< EObjectTypeQuery>> layerDodge;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Flank", BlueprintReadOnly, meta = (AllowPrivateAccess))
	int percentageFlank = 50;
	UPROPERTY(VisibleAnywhere, Category = "Trapper_Property|Flank", BlueprintReadOnly, meta = (AllowPrivateAccess))
	FPointRangeArena flankPoint;

	UPROPERTY(VisibleAnywhere, Category = "Trapper_Property|Critique", BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool isCritiqueMode = false;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique", BlueprintReadOnly, meta = (AllowPrivateAccess))
	int percentageLifeCritique = 30;
	UPROPERTY(VisibleAnywhere, Category = "Trapper_Property|Critique|Flank", BlueprintReadOnly, meta = (AllowPrivateAccess))
	int currentPercentageFlank = 50;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Camouflage")
	float speedCamouflageCritique = 800;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Movement")
	float speedWalkCritique = 700;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Movement")
	float speedRunCritique = 700;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Movement")
	float accelerationCritique = 700;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Protection")
	float protectionCritique = 41;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Dodge", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float damageDodgeCritique = 10;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Dodge", BlueprintReadOnly, meta = (AllowPrivateAccess))
	float damageTimeDodgeCritique = 500;
	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Flank", BlueprintReadOnly, meta = (AllowPrivateAccess))
	int percentageFlankCritique = 50;


	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Status", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float critiqueProtectionValue;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Status", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float critiqueStunResistanceValue;

	UPROPERTY(EditAnywhere, Category = "Trapper_Property|Critique|Status", meta = (UIMin = 0.1f, ClampMin = 0.1f))
	float critiqueStaggerResistanceValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowprivateAccess))
	bool canStartCamouflage = false;

	int currentDamageDodge = 0;
	float currentTimeDamageDodge = 0;

	float maxLife = 0;
	FTimerHandle dodgeCooldownTimer;
	float currentDamage = 0;
	bool canDodge = true;
	bool isCooldownDodge = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool camouflageMode = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<UCoverComponent*> listCoverInRange;

	UPROPERTY(ReplicatedUsing = On_StartCamouflage)
	bool startCamouflage = false;
	UPROPERTY(ReplicatedUsing = On_StartDisableCamouflage)
	bool startDisableCamouflage = false;

	TObjectPtr<UCoverComponent> currentCover = nullptr;

public:
	AAI_Hunter();

#pragma region EventMethod
	FORCEINLINE FOnStartCamouflage& OnStartCamouflage()
	{
		return onStartCamouflage;
	}
	FORCEINLINE FOnStopCamouflage& OnStopCamouflage()
	{
		return onStopCamouflage;
	}
	FORCEINLINE FOnStartDisableCamouflage& OnStartDisableCamouflage()
	{
		return onStartDisableCamouflage;
	}
	FORCEINLINE FOnStopDisableCamouflage& OnStopDisableCamouflage()
	{
		return onStopDisableCamouflage;
	}
	FORCEINLINE FOnShootTongue& OnShootTongue()
	{
		return onShootTongue;
	}
	FORCEINLINE FOnCritique& OnCritique()
	{
		return onCritique;
	}
#pragma endregion

#pragma region Getter
	FORCEINLINE UCamouflageComponent* GetCamouflageComponent() const
	{
		return camouflageComponent;
	}
	FORCEINLINE bool GetCamouflageMode() const
	{
		return camouflageMode;
	}
	FORCEINLINE TArray<UCoverComponent*> GetCoverInRange() const
	{
		return listCoverInRange;
	}
	FORCEINLINE TObjectPtr<UCoverComponent> GetCurrentCover() const
	{
		return currentCover;
	}
	FORCEINLINE FPointRangeArena GetFlankPoints() const
	{
		return flankPoint;
	}
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetCoverInRange(const TArray<UCoverComponent*>& _coverInRange)
	{
		listCoverInRange = _coverInRange;
	}
	FORCEINLINE void SetCurrentCover(const TObjectPtr<UCoverComponent>& _cover)
	{
		currentCover = _cover;
	}

	FORCEINLINE void SetCamouflageMode(const bool _camouflageMode)
	{
		camouflageMode = _camouflageMode;
	}
	FORCEINLINE void SetFlankPoints(const FPointRangeArena& _flankPoints)
	{
		flankPoint = _flankPoints;
	}
	FORCEINLINE void SetCanStartCamouflage(const bool _canStartCamouflage)
	{
		if (_canStartCamouflage)
			stateTreeComponent->StartCamouflage();
	}
#pragma endregion


protected:
	virtual void BeginPlay() override;

private:
	void SetAI(AArena* _arena) override;
	UFUNCTION() void DamageHunter(AActor* _owner, const int _currentLife, const int _damage, AActor* _damageSource);
	void StopDodge();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	void CritiqueMode();

#pragma region Animation
private:
	UFUNCTION() void On_StartCamouflage();
	UFUNCTION() void On_StartDisableCamouflage();
	UFUNCTION(BlueprintCallable) void StopPlayCamouflageAnimation();
	UFUNCTION(BlueprintCallable) void StopPlayDisableCamouflageAnimation();
public:
	//TODO replace in TaskTransforem
	void StartPlayCamouflageAnimation();
	void StartRemoveCamouflageAnimation();
#pragma endregion Animation

};
