#pragma once

#include "CoreMinimal.h"
#include "StockMenuSettings.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/Game/Interactable/Hub/StockActor/StockInteractableActor.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Hub/Stock/WeaponInformationWidget.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "NewStock.generated.h"

class UButtonPrefab;
class UButtonMemberPrefab;
class AMembersActor;

UENUM()
enum class EWeaponType : uint8
{
	LEFT, RIGHT, CALF, THIGH, None
};

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UNewStock final : public UUI
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowInformation, const AMembersActor*, _member);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShowStock);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeMember);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseOverlay);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShowStock onShowStock;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnChangeMember onChangeMember;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnShowInformation onShowInformation;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnCloseOverlay onCloseOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> ButtonLeftArm;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> ButtonRightArm;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> ButtonCalf;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> ButtonThigh;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> backButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UOverlay> overlay;

#pragma region left side
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	//TObjectPtr<UHorizontalBox> HBLeft;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> LeftArm;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> Calf;
#pragma endregion left side

#pragma region right side
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	//TObjectPtr<UHorizontalBox> HBRight;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> RightArm;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> Thigh;
#pragma endregion right side

	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	TObjectPtr<UStockMenuSettings> settings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWeaponInformationWidget> LeftInformation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWeaponInformationWidget> RightInformation;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UMediaPlayer> media;

	UPROPERTY()
	TObjectPtr<AMembersActor> temporary = nullptr;

	UPROPERTY()
	TObjectPtr<AStockInteractableActor> parent = nullptr;

	UPROPERTY()
	TMap<EWeaponType, TObjectPtr<UButtonMemberPrefab>> buttonMember;

	UPROPERTY()
	bool isLeft = false;

	UPROPERTY()
	EWeaponType overlayType;

public:
	FORCEINLINE TObjectPtr<UButtonMemberPrefab> GetButtonMember(EWeaponType _weaponType) const
	{
		return buttonMember.Contains(_weaponType) ? buttonMember[_weaponType] : nullptr;
	}

	FORCEINLINE FOnShowStock& OnShowStock() { return onShowStock; }

	FORCEINLINE void SetMemberSelectedTemporary(const TObjectPtr<AMembersActor>& member)
	{
		temporary = member;
		ShowInformation();
	}

	void SetMemberButtonSelected(EWeaponType _weaponType, const TObjectPtr<UButtonMemberPrefab>& _member);

	void CloseOverlay();

	FORCEINLINE FOnChangeMember& OnChangeMember() { return onChangeMember; }
	UFUNCTION()
	FORCEINLINE void Callback_OnChangeMember() { onChangeMember.Broadcast(); }

	FORCEINLINE void SetOwner(AStockInteractableActor* StockInteractableActor) { parent = StockInteractableActor; }

private:
	UFUNCTION()
	FORCEINLINE void Callback_OnShowStock() { onShowStock.Broadcast(); }

	/**
	 * CLose overlay when clicked on empty
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	void CloseOverlayFromUI();
	/**
	 * Close and clear all weapon information
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|UI")
	FORCEINLINE void CloseSelection()
	{
		media->Close();
		temporary = nullptr;
		media = nullptr;
	}

	/**
	 * Check is overlay is visible or not
	 * @return true of false
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|UI")
	FORCEINLINE bool IsOverlayVisible() const { return overlay->GetVisibility() == ESlateVisibility::Visible; }

	/**
	 * Show overlay of information about of button
	 * @param _visible The visibility of overlay
	 * @param _weaponType
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|UI")
	void ShowOverlay(bool _visible, const EWeaponType _weaponType = EWeaponType::None);
	UFUNCTION()
	void OpenOverlayLeftArm();
	UFUNCTION()
	void OpenOverlayRightArm();
	UFUNCTION()
	void OpenOverlayThigh();
	UFUNCTION()
	void OpenOverlayCalf();

	virtual void InitWithDelay() override;
	UFUNCTION()
	void Back();
	virtual void RegisterEvents() override;
	virtual void Refresh() override;
	virtual void HandleInputChanged(bool isGamepad) override;


	void HideAll() const;
	void LoadMembers();
	void InitializeLeftArms();
	void InitializeRightArms();
	void InitializeLeftLegs();
	void InitializeRightLegs();
	void LoadWeaponInformation(const TObjectPtr<UVerticalBox>& Object);
	void ShowInformation(EWeaponType _weaponType = EWeaponType::None);
	void InitializeMemberInList(const TSubclassOf<AMembersActor>& _actor, const TObjectPtr<UVerticalBox>& _list);
};
