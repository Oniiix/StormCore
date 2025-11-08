#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/ATH/Game/Cooldown/CooldownWeapon.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"
#include "newPlayerData.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UNewPlayerData final : public UUI
{
	GENERATED_BODY()

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> playerName;*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UProgressBar> lifebar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> lifeInformation;

	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USpacer> spacerPlayer;*/

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText lifeFormat = TEXTSTR("{0}/{1}");

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString formatName = "<normal>{0}</>";

	UPROPERTY()
	TObjectPtr<AMainCharacter> currentPlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCooldownWeapon> PowerLeftArm;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UCooldownWeapon> PowerCalfCooldown;

	UPROPERTY()
	TObjectPtr<ULifeComponent> lifeComponent;


public:
	void SetupPlayer(const TObjectPtr<AMainCharacter>& Player);
	UFUNCTION(BlueprintCallable)
	void RefreshLifeBar(AActor* _owner, const int _currentLife, const int _change, AActor* _damageSource);
	void RefreshAllInputForSkills() const;

	FORCEINLINE TObjectPtr<UCooldownWeapon> GetLeftArmCooldown() const { return PowerLeftArm; }
	FORCEINLINE TObjectPtr<UCooldownWeapon> GetCalfCooldown() const { return PowerCalfCooldown; }
private:
	FString GetLifeFormated() const;
};
