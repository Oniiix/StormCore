// 

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "RobotHunter/CCC/Character/Components/Bonus/BonusComponent.h"
#include "RobotHunter/UI/Template/Prefab/Prefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "CardTemplate.generated.h"

struct FBonusStruct;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCardTemplate : public UPrefab
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoverCard);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnHoverCard);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardSelected);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCardUnSelected);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnBonusLoad, FString, _name, FString, description, FLinearColor,
	                                              color,
	                                              FSlateBrush, brush);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHoverCard onHoverCard;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnCardSelected OnCardSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnCardUnSelected OnCardUnSelected;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnBonusLoad OnBonusLoad;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnUnHoverCard onUnHoverCard;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UOverlay> newBonusOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButton> buttonCard;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> titleBonus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> logoBonus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> descriptionBonus;

	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess))
	bool bIsSelected = false;

	EBonusType bonusType;

	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	FString TextBonusFormat = "<normal>{0}</>";
	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta= (AllowPrivateAccess))
	FString DescriptionBonusFormat = "<normal>{0}</>";

public:
	FORCEINLINE EBonusType GetBonusType() const { return bonusType; }

	UFUNCTION()
	void SelectCard();
	UFUNCTION()
	void PlayAnimationUnHover();
	UFUNCTION()
	void PlayCardSelectedAnimation();
	UFUNCTION(BlueprintCallable)
	void UnSelectCard();
	virtual void RegisterEvents() override;


	void SetIsNew(const bool bIsNew) const;
	void SetBonus(const FBonusStruct& Bonus);

private:
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;

	UFUNCTION()
	void PlayAnimationHover();
};
