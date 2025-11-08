#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Hub/StockMenu/NewStock.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "ButtonMemberPrefab.generated.h"

#define BUTTON_MEMBER_USE_LOG false

class AMembersActor;

UCLASS(HideDropdown)
class ROBOTHUNTER_API UButtonMemberPrefab final : public UButtonPrefab
{
	GENERATED_BODY()

#pragma region animation

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CanEquipAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EquipAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> EquippedAnimation;

	UPROPERTY()
	TObjectPtr<UUMGSequencePlayer> LoadingAnimationForwardPlayer;

	UPROPERTY()
	TObjectPtr<UUMGSequencePlayer> LoadingAnimationReversePlayer;

#pragma endregion animation

	/** Time in seconds to animation played */
	UPROPERTY(EditAnywhere, Category = "Custom")
	float timeToBuyInSeconds = 1.0f;

	UPROPERTY()
	TSubclassOf<AMembersActor> currentMemberReference;

	UPROPERTY()
	TObjectPtr<AMembersActor> currentMember;

	UPROPERTY()
	UNewStock* menu;

	UPROPERTY()
	TMap<UClass*, EWeaponType> handlers;


	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "Custom")
	bool isEquip = false;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString format = "<normal>{0}</>";

public:
	UButtonMemberPrefab();

	FORCEINLINE bool IsEquipped() const { return isEquip; }
	FORCEINLINE TObjectPtr<AMembersActor> GetCurrentMember() const { return currentMember; };

	void SetMember(const TSubclassOf<AMembersActor>& member, const TObjectPtr<UNewStock>& stockMenu);

private:
	FORCEINLINE void SetEquip(bool _bCond) { isEquip = _bCond; }

	//Equip member
	UFUNCTION()
	void EquipMember();
	UFUNCTION()
	void HoverInformation();
	UFUNCTION()
	void ClearInformation();

	virtual void RegisterEvents() override;

	void EquipMember(const TObjectPtr<ACustomPlayerController>& _playerController);

	bool IsEquipped(const TObjectPtr<AMembersActor>& _member, const TObjectPtr<ACustomPlayerController>& _pc) const;

	EWeaponType GetType() const;
};
