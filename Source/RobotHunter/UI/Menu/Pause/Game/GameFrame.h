// 

#pragma once

#include "CoreMinimal.h"
#include "BonusInformation.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/UI/Menu/Pause/PauseFrame.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Template/Prefab/RowBonus/PrefabRowBonus.h"
#include "GameFrame.generated.h"

class UCustomGameInstance;
class UGameSectionSettings;
class UPlayerSession;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UGameFrame final : public UPauseFrame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UPlayerSession> players;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> quitBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> backBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> backHUBBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> OptionBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> NoBonusText;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UGameSectionSettings> settings;

	UPROPERTY()
	TObjectPtr<UCustomGameInstance> GameInstance;

	UPROPERTY(EditAnywhere, Category = "Custom")
	TSubclassOf<UPrefabRowBonus> rowPrefabBonus;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> BonusList;

	UPROPERTY(VisibleAnywhere)
	TArray<FBonusInformation> bonusStructList;
	UPROPERTY(EditAnywhere, Category = "Custom")
	FName mainLevel;

protected:
	virtual void Init() override;

	virtual void RegisterEvents() override;
	virtual void Refresh() override;

private:
	UFUNCTION()
	void Back();
	UFUNCTION()
	void QuitSession();
	//UFUNCTION()
	//void QuitTutorial();
	UFUNCTION()
	void Quit();
	UFUNCTION()
	void BackHUB();
	UFUNCTION()
	void ShowOption();
	UFUNCTION()
	void AddBonus(const FBonusStruct& Bonus);

	virtual void DisableAllWidgetInteractable() override;
	virtual void HandleInputChanged(bool isGamepad) override;

	void RefreshBonusList();

	UFUNCTION(BlueprintCallable)
	bool IsHUB() const;
};
