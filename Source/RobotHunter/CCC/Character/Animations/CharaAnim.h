// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <RobotHunter/CCC/Character/MainCharacter.h>
#include "CharaAnim.generated.h"

#define CHARA_ANIM_USE_LOG false

UCLASS()
class ROBOTHUNTER_API UCharaAnim : public UAnimInstance
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopCarrying);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunch);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnStopCarrying onStopCarrying;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLaunch onLaunch;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess))
		TObjectPtr<AMainCharacter> chara = nullptr;

	UPROPERTY( EditAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess))
		FVector2D speed;
	UPROPERTY( EditAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess))
		FVector moveDir;

	UPROPERTY( EditAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess))
		bool isJumping = false;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
		bool isScoping = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool canStrafeTurn = false;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool hasRightArmShoot = false;
		UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool hasLeftArmShoot = false;
		UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool hasLeftArmRelease = false;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool isHeavy = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool isOverHeat = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
		bool isCool = false;

	UPROPERTY( EditAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess))
	int numAnim = 0;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasStartKick = false;
	UPROPERTY( EditAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess))
	bool hasReachKick = false;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasPickUp = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasCarry = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isHolding = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasInteract = false;

	int weightToken = 0;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	float animWeight = 0.0f;

	bool lerpAnimWeight = false;
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	float targetAnimWeight = 0.0f;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	float lerpSpeed = 10.0f;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasStartWCKick = false;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	float handcarSpeed = 0;
	UPROPERTY( EditAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess))
	bool isUsingBrake = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool useBrake = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isKicking = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isUsingLifeStock = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isUsingEggStock = false;

	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasStartSKKick = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool hasSpawnBox = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isBumped = false;


	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isOpenRA = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isOpenLA = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isOpenThigh = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	bool isOpenCalf = false;
	UPROPERTY( EditAnywhere, BluePrintReadWrite, meta = (AllowPrivateAccess))
	float strafeYaw = 0.f;


public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void DrawSphereAroundCharacter();

	UFUNCTION() void Bind();

	void BindCalfs();

	void BindLeftArms();

	void BindRightArms();

	UFUNCTION(Server,Unreliable) void ServerRpc_SetSpeed(FVector2D _speed);
	UFUNCTION() void SetSpeed(FVector2D _speed);
	UFUNCTION() void UpdateMovement(const FVector2D& _speed, const FVector& _moveDir);
	UFUNCTION(Server, Unreliable) void Client_SetIsJumping(bool _status);
	UFUNCTION() void SetIsJumping(bool _status);
	UFUNCTION(Server, Reliable) void Client_SetIsScoping(bool _status);
	UFUNCTION() void SetIsScoping(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetIsHeavy(bool _status);
	UFUNCTION() void SetIsHeavy(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetIsCool(bool _status);
	UFUNCTION() void SetIsCool(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetIsOverHeat(bool _status);
	UFUNCTION() void SetIsOverHeat(bool _status);
	UFUNCTION(Server, Unreliable) void Client_UpdateRecoil(bool _status,int _numAnim);
	UFUNCTION() void UpdateRecoil(bool _status,int _numAnim);
	UFUNCTION(Server, Unreliable) void Client_SetHasLeftArmShoot(bool _status);
	UFUNCTION() void SetHasLeftArmShoot(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetHasStartKick(bool _status);
	UFUNCTION() void SetHasStartKick(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetHasReachKick(bool _status);
	UFUNCTION() void SetHasReachKick(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetHasStartWCKick(bool _status);
	UFUNCTION() void SetHasStartWCKick(bool _status);
	UFUNCTION(Server, Unreliable) void Client_SetHasInteract(bool _status);
	UFUNCTION() void SetHasStartSKKick(bool _status);
	UFUNCTION() void SetHasSpawnBox(bool _status);
	UFUNCTION() void SetHasInteract(bool _status);
	UFUNCTION() void SetIsBumped(bool _status, const bool _isBumpByAI);
	UFUNCTION() void SetHasPickUp(bool _status);
	UFUNCTION() void SetHasCarry(bool _status);
	UFUNCTION() void SetLeftArmReleased(bool _status);
	UFUNCTION() void SetIsKicking(bool _status);
	UFUNCTION() void SetIsUsingBrake(bool _status);
	UFUNCTION() void SetIsUsingLifeStock(bool _status);
	UFUNCTION() void SetIsUsingEggStock(bool _status);
	UFUNCTION() void SetHandcarSpeed(float _speed);
	UFUNCTION() void SetIsOpenRA(bool _status);
	UFUNCTION() void SetIsOpenLA(bool _status);
	UFUNCTION() void SetIsOpenThigh(bool _status);
	UFUNCTION() void SetIsOpenCalf(bool _status);
	UFUNCTION() void SetCanStrafeTurn(bool _status);
	UFUNCTION() void SetStrafeYaw(float _yaw);

	UFUNCTION(BlueprintCallable) void SetAnimWeight(float _weight, const bool _lerpAnimWeight = true);
	UFUNCTION(BlueprintCallable) void ClearAllAnimWeight(const bool _lerpAnimWeight = true);


	UFUNCTION(BlueprintCallable) bool GetCanStrafeTurn() const { return canStrafeTurn; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsScoping() const { return isScoping; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsBumped() const { return isBumped; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetHasLeftArmShoot() const { return hasLeftArmShoot; }
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetHasLeftArmRelease() const { return hasLeftArmRelease; }
	UFUNCTION(BlueprintCallable) float GetSpeed();
	UFUNCTION(BlueprintCallable) bool GetIsUsingBrake();
	UFUNCTION(BlueprintCallable) bool GetIsKicking();
	UFUNCTION(BlueprintCallable) FORCEINLINE bool GetIsHolding() const { return isHolding; }

	FORCEINLINE FOnLaunch& OnLaunch() { return onLaunch; }
	

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
