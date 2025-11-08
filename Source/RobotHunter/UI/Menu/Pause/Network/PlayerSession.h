#pragma once

#include "CoreMinimal.h"
#include "PlayerSessionSettings.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "PlayerSession.generated.h"

#define GAME_PAUSE_MENU_USE_LOG false

class AMainCharacter;

USTRUCT()
struct FConnectionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (UIMin = 0, ClampMin = 0, ClampMax = 999, UIMax= 999))
	float pingMax = 0.0f;

	UPROPERTY(EditAnywhere)
	UTexture2D* texture = nullptr;
};

UENUM()
enum EConnectionStatePlayer
{
	None UMETA(DisplayName = "Choose a state"),
	No_Connection UMETA(DisplayName = "No Connection"),
	One_Bar UMETA(DisplayName = "One bar"),
	Two_Bar UMETA(DisplayName = "Two Bar"),
	Three_Bar UMETA(DisplayName = "Three Bar"),
	Full UMETA(DisplayName = "Full Bar")
};


UCLASS(HideDropdown)
class ROBOTHUNTER_API UPlayerSession final : public UUI
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Custom")
	TMap<TEnumAsByte<EConnectionStatePlayer>, FConnectionData> StateConnection = {};

	UPROPERTY()
	TArray<AMainCharacter*> players = {};

#pragma region firstPlayer
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> FirstPlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textNameFirstPlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> FPdp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> FConnection;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> FPing;

#pragma endregion firstPlayer

#pragma region secondPlayer
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UVerticalBox> SecondPlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> textNameSecondPlayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> SPdp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UImage> SConnection;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextPrefab> SPing;
#pragma endregion secondPlayer

	UPROPERTY(EditAnywhere, Category = "Custom")
	TObjectPtr<UPlayerSessionSettings> settings;

	UPROPERTY()
	float currentTime = 0.f;

public:
	virtual void Refresh() override;

private:
	FORCEINLINE UTexture2D* GetTextureFromConnectionState(EConnectionStatePlayer connectionState)
	{
		if (!StateConnection.Contains(connectionState))
		{
			return StateConnection[No_Connection].texture;
		}
		return StateConnection[connectionState].texture;
	}


	UFUNCTION()
	void AddPlayerToList(AMainCharacter* _player);


	virtual void RegisterEvents() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	void RefreshDelay();
	void LoadInformationPlayer(const TObjectPtr<APlayerState>& _player, const TObjectPtr<UTextPrefab>& _text,
	                           const TObjectPtr<UImage>& _pdp) const;
	void RefreshPings(const APlayerState* _player, UImage* _pingImage, UTextPrefab* _pingText);
	void EnableLayerPlayer(UVerticalBox* _layer, bool _visibility) const;
};
