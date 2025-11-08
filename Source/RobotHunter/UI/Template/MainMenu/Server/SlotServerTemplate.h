#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Network/OnlineManagerSubsystem.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Template/Prefab/TextPrefab/TextPrefab.h"

#include "SlotServerTemplate.generated.h"

class UListServerMenu;
/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API USlotServerTemplate final : public UUI
{
	GENERATED_BODY()

#pragma region properties

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextPrefab> serverName;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextPrefab> owner;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextPrefab> date;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UTextPrefab> money;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UButtonPrefab> serverButton;

	/** Set the size maximum of string to server name*/
	UPROPERTY(EditAnywhere, Category = "Custom|UI")
	int lengthOfMaxSizeText = 15;

	FString sessionName;
	FTimerHandle timerLoading;

	UPROPERTY()
	const UListServerMenu* ownerClass;


	UPROPERTY(EditAnywhere, Category = "Custom")
	FString serverNameFormat = "<normal>{0}</>";


	UPROPERTY(EditAnywhere, Category = "Custom")
	FString ownerNameFormat = "<normal>{0}</>";

#pragma endregion properties

#pragma region private

	/**
	 * Truncates a string if it is too long and assigns it to a text widget, adding ��� at the end if necessary.
	 * @param _nameServer FString
	 * @param _widget  UTextBlock*
	 * @param _format
	 */
	void LoadTextWidgetWithClamp(FString _nameServer, UTextPrefab* _widget, const FString& _format) const;

	/**
	*
	*/
	UFUNCTION()
	void Connect();
	UFUNCTION()
	void LaunchMap();
#pragma endregion private

public:
	/**
	 * Init the data of template with struct of server data
	 * @param _owner UListServerMenu*
	 * @param _server FServerResult
	 */
	void InitServer(const UListServerMenu* _owner, FServerResult _server);

	FORCEINLINE TObjectPtr<UButtonPrefab> GetButton() const { return serverButton; }
};
