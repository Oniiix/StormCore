#pragma once

#include "CoreMinimal.h"
#include "../../BaseMenu.h"
#include "Components/CircularThrobber.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Template/Prefab/CheckBoxPrefab/CheckboxPrefab.h"
#include "RobotHunter/UI/Template/Prefab/InputFieldPrefab/InputFieldPrefab.h"
#include "RobotHunter/UI/Template/Prefab/ScrollBoxPrefab/ScrollboxPrefab.h"
//#include "RobotHunter/UI/Template/Prefab/InputFieldPrefab/InputFieldPrefab.h"
//#include "RobotHunter/UI/Template/Prefab/ScrollboxPrefab/ScrollboxPrefab.h"
#include "ListServerMenu.generated.h"

class USlotServerTemplate;
class UListServerSettings;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UListServerMenu final : public UBaseMenu
{
	GENERATED_BODY()

#pragma region properties
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> backButton;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> refreshButton;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextPrefab> labelNumServer;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UScrollboxPrefab> scrollServersList;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UCheckboxPrefab> checkFriendsOnly;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UCircularThrobber> circleLoad;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UInputFieldPrefab> inputServer;

	/** Settings of menu (text, template...) */
	UPROPERTY(EditAnywhere, Category = "Custom|Settings")
	TObjectPtr<UListServerSettings> settings;

	UPROPERTY()
	TObjectPtr<UOnlineManagerSubsystem> oms;

	TArray<USlotServerTemplate*> serversList;

	TArray<FServerResult> servers;

#pragma endregion properties


#pragma region methods
#pragma region private
	/**
 * Go back to the main menu
 */
	UFUNCTION()
	void Back();

	/**
	 * Method called by the OnlineManager Subsystem to load the servers
	 * @param _servers List of servers
	 */
	UFUNCTION()
	void ServerFound(const TArray<FServerResult>& _servers);

	/**
	 * Allows you to search for a server by filter
	 * @param _newInput The filter text
	 */
	UFUNCTION()
	void FindServer(const FString& _newInput);


	/**
	 * Refresh servers
	 * @param _friendOnly bool
	 */
	UFUNCTION()
	void RefreshServer(bool _friendOnly);

	/**
	 * Create a widget from the template for the server
	 * @param _result Server settings
	 */
	void CreateWidgetServer(const FServerResult& _result) const;


	/**
	 * Load list of server with a list of server result from Online
	 * @param _servers const TArray<FServerResult>& 
	 */
	void LoadListServers(const TArray<FServerResult>& _servers) const;

#pragma endregion private

#pragma region protected

protected:
	virtual void RegisterEvents() override;
#pragma endregion protected

#pragma region public

public:
	/**
	 * Refresh the server list
	 */
	virtual void Refresh() override;
	virtual void Init() override;
#pragma endregion public

#pragma endregion methods
};
