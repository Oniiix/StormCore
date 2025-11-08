// 

#pragma once

#include "CoreMinimal.h"
#include "Components/InputKeySelector.h"
#include "RobotHunter/Game/GPE/Members/MembersActor.h"
#include "KeyImageDataAsset.h"
#include "RobotHunter/UI/UI.h"
#include "CooldownWeapon.generated.h"

class UInputSubsystem;


UCLASS()
class ROBOTHUNTER_API UCooldownWeapon : public UUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseWeapon, const float, _cooldown);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUseWeapon onUseWeapon;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	TObjectPtr<AMembersActor> member;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> imageWeapon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UInputKeySelector> input;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> imageInput;

	UPROPERTY()
	UInputSubsystem* subsystem;

private:
	virtual void Init() override;
	virtual void RegisterEvents() override;
	virtual void HandleInputChanged(bool isGamepad) override;

	void SetupKey(const FKey& _key);

public:
	UFUNCTION()
	void RefreshInput(bool _isGamepad = false);
	void SetupMember(AMembersActor* _member);
	void Use(const float _cooldown) const;
};
