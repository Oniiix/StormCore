// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RobotHunter/Utils/ENUM_AI.h"
#include "AIBaseAnimInstance.generated.h"

#define AI_ANIM_USE_LOG false
#define FLFOOT_SOCKET "FL_Foot"
#define FRFOOT_SOCKET "FR_Foot"
#define BLFOOT_SOCKET "BL_Foot"
#define BRFOOT_SOCKET "BR_Foot"

class AAI_Base;
class UCharacterMovementComponent;
class UMovableComponent;

UCLASS(ABSTRACT)
class ROBOTHUNTER_API UAIBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndAnimation);
	FOnEndAnimation onEndAnimation; /// For all anim
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndJump);
	FOnEndJump onEndJump; 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndStun);
	FOnEndStun onEndStun; 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnableHitBox);
	FOnEnableHitBox onEnableHitBox;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableHitBox);
	FOnDisableHitBox onDisableHitBox;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpAttack);
	FOnJumpAttack onJumpAttack; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI_Animation", meta = (AllowPrivateAccess))
	TObjectPtr<AAI_Base> owner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI_Animation", meta = (AllowPrivateAccess))
	float currentSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI_Animation", meta = (AllowPrivateAccess))
	float currentAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool canPlayAnimation = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isOnGround = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isStun = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isSpawning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isCrying = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isBumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool jumpAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|State", meta = (AllowPrivateAccess))
	bool isDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|IK", meta = (AllowPrivateAccess))
	bool stopIK = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|IK", meta = (AllowPrivateAccess))
	FVector forwardLeftFootLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|IK", meta = (AllowPrivateAccess))
	FVector forwardRightFootLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|IK", meta = (AllowPrivateAccess))
	FVector backwardLeftFootLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|IK", meta = (AllowPrivateAccess))
	FVector backwardRightFootLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI_Animation|IK", meta = (AllowPrivateAccess))
	FRotator headRotation = FRotator::ZeroRotator;

	TObjectPtr<UCharacterMovementComponent> characterMovementComponent = nullptr;
	TObjectPtr<UMovableComponent> movableComponent = nullptr;
	TObjectPtr<USkeletalMeshComponent> skeletalMesh = nullptr;
	TArray<TEnumAsByte<EObjectTypeQuery>> layersGround;
	bool isInitializationGood = false;

	void NativeBeginPlay() override;
	virtual void InitAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void CalculateIK();

	UFUNCTION() 
	void Enable(bool _enable);
	UFUNCTION()
	void EndStun();
	UFUNCTION()
	void Bump();
	UFUNCTION()
	void OnDead(AAI_Base* _owner);

	UFUNCTION(BlueprintCallable) 
	void EnableHitBox();
	UFUNCTION(BlueprintCallable) 
	void DisableHitBox();
	UFUNCTION(BlueprintCallable) 
	void JumpAttack();

public:
	FORCEINLINE FOnEndAnimation& OnEndAnimation() { return onEndAnimation; }
	FORCEINLINE FOnEndJump& OnEndJump() { return onEndJump; }
	FORCEINLINE FOnEndStun& OnEndStun() { return onEndStun; }
	FORCEINLINE FOnEnableHitBox& OnEnableHitBox() { return onEnableHitBox; }
	FORCEINLINE FOnDisableHitBox& OnDisableHitBox() { return onDisableHitBox; }
	FORCEINLINE FOnJumpAttack& OnJumpAttack() { return onJumpAttack; }

	FORCEINLINE void SetCurrentAngle(const float _value) { currentAngle = _value; }

	UFUNCTION()
	void StartStun(const float _stunTime);

	void Jump();
	UFUNCTION(BlueprintCallable) 
	void EndJump();

	virtual void LaunchAnimation(const EAIAnimation& _animation);
	UFUNCTION(BlueprintCallable) 
	virtual void EndAnimation();
};

