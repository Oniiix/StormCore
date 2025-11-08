// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "PlayerMappableKeySettings.h"
#include "GameFramework/Actor.h"
#include "../../../CustomActor.h"
#include "../../../UI/ATH/Reticle/Reticle.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/Components/Status/Status.h"
#include "MembersActor.generated.h"

class UInputMappingContext;
class UCrosshair;

UENUM()
enum EMemberType
{
	NONE,
	NAILGUN,
	FIREFLIES,
	CADENCESHOTGUN,
	SNAPFLASH,
	ZEUSBATTERYBOLT,
	CABLEUR,
	HAULER,
	OFFROADROLLERDRIVES,
	PHAZERBEACON,
	EJECTKICK,
	WCKICK,
	SEISMICKICK
};


class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API AMembersActor : public ACustomActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPress);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRelease);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeactivatedStatus, bool, hasDeactivatedStatus);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownStart, const float, _maxCooldown);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooldownEnd);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, const FHitResult&, _results);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShoot, const FVector&, _startPosition, const FVector&, _direction);

protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnPress onPress;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnRelease onRelease;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnReset onReset;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnHit onHit;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnShoot onShoot;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnCooldownStart onCooldownStart;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnCooldownEnd onCooldownEnd;


	UPROPERTY(EditAnywhere, Category = "Custom|Property|Charge")
	bool useCharge;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Charge", meta = (EditCondition = "useCharge"))
	float maxChargeTime;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Cooldown")
	bool useCooldown;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Cooldown", meta = (EditCondition = "useCooldown"))
	float cooldown;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Cooldown", meta = (EditCondition = "useCooldown"))
	int uses;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Camera")
	FString cameraKey;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Stock")
	FText name = EMPTY_TEXT;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Stock")
	FText description = EMPTY_TEXT;

	UPROPERTY(EditAnywhere, Category = "Custom|Game|ATH|Input")
	TObjectPtr<UInputMappingContext> inputContext;

	UPROPERTY(EditAnywhere, Category = "Custom|Game|ATH|Input")
	TObjectPtr<UInputAction> inputAction;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Stock|Video")
	TObjectPtr<UMediaPlayer> mediaPlayerMember;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Stock|Video")
	TObjectPtr<UMediaSource> videoMember;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Stock|Video")
	TObjectPtr<UMaterialInterface> material;

	UPROPERTY(EditAnywhere, Category = "Custom|Game|ATH")
	TObjectPtr<UMaterialInterface> icon;

	UPROPERTY(EditAnywhere, Category = "Custom|Game|ATH|CrossHair")
	TSubclassOf<UReticle> crossHair = nullptr;

	UPROPERTY(EditAnywhere, Category = "Custom|Game|ATH|CrossHair")
	bool hasAlternative = false;

	UPROPERTY(EditAnywhere, Category = "Custom|Game|ATH|CrossHair",
		meta = (EditCondition = "hasAlternative", EditConditionHides))
	TSubclassOf<UReticle> crossHairAlternative = nullptr;

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (AllowPrivateAccess))
	TObjectPtr<AMainCharacter> mainCharacter = nullptr;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnDeactivatedStatus onDeactivatedStatus;


	//Use-Bool
	bool hasDeactivatedStatus;
	bool canShot;
	bool isPressed = false;

	bool isActivated = true;

	UPROPERTY(VisibleAnywhere)
	int activationToken = 0;


	//Charge
	float currentChargeTime;
	int currentUses;
	FTimerHandle chargeTimer;


	//Cooldown
	float cooldownMultiplier;
	float cooldownBonus;
	FTimerHandle cooldownTimer;

	//Bonus
	float bonusDamage;

	float bleedRate;
	FStatusParams bleedBonusParams;


	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMemberType> memberType;

public:
	void SetIsActivated(const bool& _status);
	FORCEINLINE bool GetIsActivated() { return isActivated; }
	FORCEINLINE bool GetCanShot() { return canShot; }

	FORCEINLINE FOnPress& OnPress() { return onPress; }
	FORCEINLINE FOnRelease& OnRelease() { return onRelease; }
	FORCEINLINE FOnReset& OnReset() { return onReset; }
	FORCEINLINE FOnCooldownEnd& OnCooldownEnd() { return onCooldownEnd; }

	FORCEINLINE TObjectPtr<UMaterialInterface> GetIcon() const { return icon; }

	FString GetInputMappingName(const bool _getGamepad = false) const;


	/**
	 * Get the media player of the member
	 * @return An Object of media player
	 */
	FORCEINLINE UMediaPlayer* GetMediaPlayerMember() const { return mediaPlayerMember; }

	/**
	 * Get the video presentation of the member
	 * @return An Object of media player
	 */
	FORCEINLINE UMediaSource* GetVideoOfMember() const { return videoMember; }

	/**
	 * Get the video presentation of the member
	 * @return An Object of media player
	 */
	FORCEINLINE UMaterialInterface* GetMaterialVideo() const { return material; }

	FORCEINLINE bool GetHasDeactivatedStatus() const { return hasDeactivatedStatus; }

	FORCEINLINE FString GetCameraKey() const { return cameraKey; }
	FORCEINLINE float GetCooldownRatio() const { 
		if (GetWorld()->GetTimerManager().IsTimerActive(cooldownTimer))
			return GetWorld()->GetTimerManager().GetTimerElapsed(cooldownTimer) / (GetWorld()->GetTimerManager().GetTimerElapsed(cooldownTimer) + GetWorld()->GetTimerManager().GetTimerRemaining(cooldownTimer));
		else
			return 0.0f;
	}
	FORCEINLINE FText GetMemberName() const { return name; }
	FORCEINLINE FText GetMemberDescription() const { return description; }
	FORCEINLINE virtual TArray<USkeletalMeshComponent*> GetMemberMeshs() { return {}; }

	FORCEINLINE TSubclassOf<UReticle> GetCrossHair() const { return crossHair; }
	FORCEINLINE TSubclassOf<UReticle> GetCrossHairAlternative() const { return crossHairAlternative; }
	FORCEINLINE TEnumAsByte<EMemberType> GetMemberType() const { return memberType; }

	FORCEINLINE FOnDeactivatedStatus& GetOnDeactivatedStatus() { return onDeactivatedStatus; }
	FORCEINLINE FOnCooldownStart& OnCooldownStart() { return onCooldownStart; }
	FORCEINLINE FOnHit& OnHit() { return onHit; }
	FORCEINLINE FOnShoot& OnShoot() { return onShoot; }

	FORCEINLINE bool GetIsPressed() const { return isPressed; }
	FORCEINLINE void SetIsPressed(const bool _isPressed) { isPressed = _isPressed; }

	FORCEINLINE void SetBonusDamage(const float _bonus) { bonusDamage = 1.0f + _bonus; }

	FORCEINLINE void SetBleedBonusParams(const float _value, const float _duration, const float _bleedRate)
	{
		bleedBonusParams.value = _value;
		bleedBonusParams.duration = _duration;
		bleedRate = _bleedRate;
	}

public:
	AMembersActor();

	void CallOnHit(const FHitResult& _results);
	void CallOnShoot(const FVector& _startPosition, const FVector& _direction);

private:
	void UpdateCurrentCharge();

protected:
	virtual void StartCooldown();
	virtual void EndCooldown();

	void StartCooldownAfterModification(const float _oldCooldown);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void AddBleed(AActor* _actor);

public:
	virtual void PressBehavior();
	virtual void ReleaseBehavior();
	virtual bool CanUse();
	virtual void ResetMember();

public:
	virtual void InitMember()
	{
	}

	virtual void SetupMember()
	{
	}

	virtual void ModifyCooldownMultiplier(const float _cooldownReducPercent);
	virtual void ModifyCooldownBonus(const float _cooldownReducPercent);

	void SetHasDeactivatedStatus(const bool _hasStatus);

	void SetMainCharacter(AMainCharacter* _chara);
	FORCEINLINE AMainCharacter* GetMainCharacter() const { return mainCharacter; }

private:
	UFUNCTION(Server, Unreliable)
	void ServerRpc_CallOnHitEvent(const FHitResult& _result);
	UFUNCTION(NetMulticast, WithValidation, Unreliable)
	void ClientRpc_CallOnHitEvent(const FHitResult& _result);

	UFUNCTION(Server, Unreliable)
	void ServerRpc_CallOnShootEvent(const FVector& _startPosition, const FVector& _direction);
	UFUNCTION(NetMulticast, WithValidation, Unreliable)
	void ClientRpc_CallOnShootEvent(const FVector& _startPosition, const FVector& _direction);
};
