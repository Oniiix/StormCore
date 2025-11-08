// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "RobotHunter/Game/GPE/Members/ThighActors/ThighActors.h"
#include "RobotHunter/UI/UI.h"
#include "BaseReticle.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UBaseReticle final : public UUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHide);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShow);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCooldownValue, float, _value);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUpdateCooldownValue onUpdateCooldownValue;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHide onHide;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow onShow;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMaterial> sprintMaterial;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMaterial> dashMaterial;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> reticle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> cooldownSprintDash;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom", meta = (AllowprivateAccess))
	TObjectPtr<UMaterialParameterCollection> MPC_HUD = nullptr;

	UPROPERTY()
	bool isReloading = false;
	/*UPROPERTY()
	float cooldown;
	UPROPERTY()
	float current;*/

	UPROPERTY(BlueprintReadWrite, meta =(AllowPrivateAccess))
	bool isShowed = false;

	//UPROPERTY(EditAnywhere, Category = "Custom")
	//float hideDelay = 2.0f;

	UPROPERTY()
	AThighActor* member;

	//float hideDelayElapsed = 0.0f;
	//bool bWaitingToHide = false;
	bool isSprint;
	//bool softReloading = false;
	//bool isFull;

public:
	virtual void Init() override;
	void SetMaterial(AThighActor* _actor);
	bool HasAlreadyTheMaterial(AThighActor* _actor);
	void HideForce() const;
	void ShowForce() const;
	void SetCooldown(const float _cooldown = 0);
	void SetCooldownValue(float X, float Stamina);

private:
	UFUNCTION()
	void EndReload();
	UFUNCTION()
	void StartReload(const float _max);
	UFUNCTION()
	void UpdateValue(const float _value, const float _max);
	UFUNCTION()
	void StartPressed();


	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void BindEvent();
};
