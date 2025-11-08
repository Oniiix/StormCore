#pragma once

#include "CoreMinimal.h"
#include "../ArmMemberActor.h"
#include "RightArmActor.generated.h"

class UCustomCameraComponent;

USTRUCT(BlueprintType)
struct FRightArmSettings
{
	GENERATED_USTRUCT_BODY()
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTestfdhgdghd);

	//UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	//FOnTestfdhgdghd onTestfdhgdghd;

	UPROPERTY(EditAnywhere)
	int damage = 10.0f;

	UPROPERTY(EditAnywhere, meta = (UMin = 0.01f, ClampMin = 0.01f, Units = "Seconds"))
	float cooldownTime = 1.0f;

	UPROPERTY(VisibleAnywhere)
	bool canShot = true;

	FTimerHandle cooldownTimer;

	FTimerDelegate cooldownDelegate;


	//FORCEINLINE FOnFinishCooldown& OnFinishCooldown() { return onFinishCooldown; }
};


UCLASS(HideDropdown)
class ROBOTHUNTER_API ARightArmActor : public AArmMemberActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUseRightArm, bool, _hasShoot, int, _numAnim);

protected:
	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	float damage = 10.f;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUseRightArm onUseRightArm;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	FRightArmSettings unscopeSettings;

	UPROPERTY(EditAnywhere, Category = "Custom|Property")
	FRightArmSettings scopeSettings;

	UPROPERTY(VisibleAnywhere, Category = "Custom|Debug")
	bool isScoped = false;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Tilt")
	float tiltMaxPitch = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Tilt")
	float tiltAddedPitch = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Tilt")
	float tiltLerpDuration = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Custom|Property|Tilt")
	float tiltReturnLerpDuration = 0.5f;

	UPROPERTY()
	TObjectPtr<UCustomCameraComponent> cameraComp;

public:
	FORCEINLINE FOnUseRightArm& OnUseRightArm() { return onUseRightArm; }
	FORCEINLINE bool GetIsScoped() const { return isScoped; }

	virtual void InitMember() override;
	virtual void ResetMember() override;

	UFUNCTION()
	void SetIsScoped(const bool _isScoped) { isScoped = _isScoped; }


	virtual void PressBehavior() override;

protected:
	void TiltShoot();
};
