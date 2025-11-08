#pragma once

#include "CoreMinimal.h"
#include "../CustomState.h"
#include "InputMappingContext.h"
#include "../../../Utils/ENUM_Speed.h"
#include "CustomPlayerState.generated.h"

class AMainCharacter;

UCLASS()
class ROBOTHUNTER_API UCustomPlayerState : public UCustomState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	FString cameraSettingsKey;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TMap<FString, FString> specificCameraSettingsKeys;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> stateInputs;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESpeedMode> stateSpeed;
	UPROPERTY()
	TObjectPtr<AMainCharacter> player;

public:
	UCustomPlayerState();

protected:
	void RetrievePlayer();
	void DisablePlayerContext();
	virtual void EnablePlayerContext();
	void UpdateCameraSettings();
	void UpdateInputMappingContext();

public:
	virtual void Enter() override;
	void UpdateStateSpeed();
};
