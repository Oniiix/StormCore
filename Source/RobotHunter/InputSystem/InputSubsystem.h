// 

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h>

#include "RobotHunter/Game/CustomGameInstance.h"
#include "InputSubsystem.generated.h"


UENUM(BlueprintType)
enum class EInputDirection : uint8
{
	Up, Down, Left, Right, None
};


/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputControllerChange, bool, isGamepad);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadKey, bool, _isGamepad);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamepadInputReceive, EInputDirection, _direction);

	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnInputControllerChange onInputControllerChange;

	UPROPERTY()
	FOnLoadKey onLoadKey;

	UPROPERTY()
	TMap<FString, FKey> keys;

	UPROPERTY()
	bool isGamepad = false;
/*
	UPROPERTY()
	TObjectPtr<UButtonPrefab> currentButtonFocused = nullptr;*/

	UPROPERTY()
	TObjectPtr<UKeyImageDataAsset> keyImageDataAsset = nullptr;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnGamepadInputReceive onInputGamepadReceive;
	
private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	
	void HandleInputChanged(ECommonInputType CommonInput);
	void RegisterEvents();

public:
	FORCEINLINE FOnInputControllerChange& OnInputControllerChange() { return onInputControllerChange; }
	FORCEINLINE FOnGamepadInputReceive& OnInputGamepadReceive() { return onInputGamepadReceive; }
	FORCEINLINE FOnLoadKey& OnLoadKey() { return onLoadKey; }
	FORCEINLINE bool IsGamepad() const { return isGamepad; }
	//FORCEINLINE TObjectPtr<UButtonPrefab> GetCurrentButtonFocused() const { return currentButtonFocused; }
	FORCEINLINE TObjectPtr<UKeyImageDataAsset> GetKeyImage() const { return keyImageDataAsset; }

	FKey FindKey(const FString& Keymapping);
	void ReplaceKey(const FString& _keyMapping, const FKey& _key);

	void LoadInput();
	//void SetCurrentFocused(UButtonPrefab* _button);
	void SetInputImage(const TObjectPtr<UKeyImageDataAsset>& Object);
	
};
