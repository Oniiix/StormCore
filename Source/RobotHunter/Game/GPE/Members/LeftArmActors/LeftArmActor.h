#pragma once

#include "CoreMinimal.h"
#include "../ArmMemberActor.h"
#include "LeftArmActor.generated.h"

UCLASS(HideDropdown)
class ROBOTHUNTER_API ALeftArmActor : public AArmMemberActor
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseLeftArm, bool, _hasShoot);
	
protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnUseLeftArm onUseLeftArm;

	UPROPERTY(VisibleAnywhere)
	bool isUsing = false;

public:
	UFUNCTION(BlueprintPure) FORCEINLINE bool GetIsUsing() { return isUsing; }
	FORCEINLINE FOnUseLeftArm& OnUseLeftArm() { return onUseLeftArm; }

	virtual void InitMember() override;
	virtual void PressBehavior() override;
	virtual void ReleaseBehavior() override;
	virtual void ResetMember() override;

	UFUNCTION() virtual void FinishUsing(bool _hasShoot);

};
