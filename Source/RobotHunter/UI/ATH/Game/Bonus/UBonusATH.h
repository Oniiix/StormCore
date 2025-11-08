// 

#pragma once

#include "CoreMinimal.h"
#include "CardTemplate.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "UBonusATH.generated.h"

class UHorizontalBox;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UBonusAth : public UUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShow);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHide);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnApplyBonus, EBonusType, bonusType);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardSelected, UCardTemplate*, OldCard, UCardTemplate*, NewCard);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnApplyBonus OnApplyBonus;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShow OnShow;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHide OnHide;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnCardSelected OnCardSwitch;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> validBonus;


	UPROPERTY()
	TArray<FBonusStruct> bonus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> HBBonus;

	UPROPERTY(EditAnywhere, Category = "Custom|Debug")
	bool enableDebug = true;

	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	UCardTemplate* CardSelected;

	UPROPERTY()
	UCardTemplate* MainWidget;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<AMainCharacter> chara = nullptr;

public:
	virtual void Init() override;
	virtual void InitWithDelay() override;
	virtual void RegisterEvents() override;
	virtual void HandleInputChanged(bool isGamepad) override;
	void SetCardSelected(UCardTemplate* CardTemplate);

private:
	UFUNCTION()
	void ApplyBonus();
	UFUNCTION()
	void RegisterBonus(TArray<FBonusStruct> _bonus);
	UFUNCTION(BlueprintCallable)
	void LaunchAnimationShow() const;
	UFUNCTION(BlueprintCallable)
	void LaunchAnimationHide() const;
	UFUNCTION(BlueprintCallable)
	void ReattributeInput() const;
};
