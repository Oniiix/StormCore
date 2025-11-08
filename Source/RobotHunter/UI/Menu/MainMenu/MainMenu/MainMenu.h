#pragma once


#include "CoreMinimal.h"
//#include "Asset/VersionDataAsset.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"

#include "MainMenu.generated.h"

class AMainMenuHUD;

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UMainMenu final : public UBaseMenu
{
	GENERATED_BODY()

#pragma region properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> solo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> coop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> options;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> credit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> quit;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextPrefab> textVersion;

	//UPROPERTY(EditAnywhere, Category = "Custom|Version")
	//TObjectPtr<UVersionDataAsset> Data;

	UPROPERTY(EditAnywhere, Category = "Custom|Version")
	FText formatVersion = TEXTSTR("<normal>{0}.{1}.{2}</>");

	
#pragma endregion properties

#pragma region methods
#pragma region private
	/**
	* Show host menu
	*/
	UFUNCTION(Blueprintable, BlueprintCallable, Category = "Custom|Event", meta = (AllowPrivateAccess))
	void ShowHostMenu();
	/**
	* Show join menu
	*/
	UFUNCTION(Blueprintable, BlueprintCallable, Category = "Custom|Event", meta = (AllowPrivateAccess))
	void ShowJoinMenu();
	/**
	* Show options menu
	*/
	UFUNCTION()
	void ShowOptionsMenu();
	/**
	* Show credits menu
	*/
	UFUNCTION()
	void ShowCreditsMenu();
	UFUNCTION()
	void QuitGameHandle();
	/**
	 * Quit game
	 */
	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void ShowSoloMenu();


	
#pragma endregion private

#pragma region protected

protected:
	virtual void Init() override;
	virtual void RegisterEvents() override;
	virtual void Refresh() override;


	//virtual void HandleInputChanged(bool isGamepad) override;
#pragma endregion protected

#pragma endregion methods
};
