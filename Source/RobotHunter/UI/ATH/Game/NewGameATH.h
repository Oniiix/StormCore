// 

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Members/MembersActor.h"
#include "RobotHunter/UI/UI.h"
#include "NewGameAth.generated.h"

class UTutorialATH;
class UDayInformation;
class UNewPlayerData;
class UPlayerData;
class UImage;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UNewGameAth : public UUI
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitShow, const float, _duration);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnHitShow onHitShow;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCanvasPanel> gameATH;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBaseReticle> reticle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UDayInformation> dayInformation;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTutorialATH> tutorialATH;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> hitMarker;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UNewPlayerData> playerData;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCooldownPrefab> cooldownThigh;*/

	UPROPERTY(EditAnywhere, Category = "Custom")
	float hitShowDuration = 1.f;

public:
	FORCEINLINE TObjectPtr<UTutorialATH> GetTutorialATH() const { return tutorialATH; }

	UFUNCTION()
	void StartCooldownLeftArm(const float _maxCooldown);
	UFUNCTION()
	void StartCooldownCalf(const float _maxCooldown);
	UFUNCTION()
	void StartCooldownThigh(const float _maxCooldown);
	UFUNCTION()
	void UseStamina(float _currentStamina, float _maxStamina);

	void LoadReticle();
	void ShowHitMarker() const;

private:
	virtual void InitWithDelay() override;
	virtual void Refresh() override;

	TSubclassOf<UReticle> GetReticleFromArm(const TObjectPtr<AMembersActor>& _arm) const;
	void LoadListOfReticle(TArray<TSubclassOf<UReticle>>& Array, const TObjectPtr<AMainCharacter>& MainCharacter) const;
	void LoadCooldown();

	UFUNCTION() void SetupMembers();
};
