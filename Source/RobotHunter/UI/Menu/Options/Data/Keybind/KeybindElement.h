// 

#pragma once

#include "CoreMinimal.h"
#include "Category/Category.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "KeybindElement.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UKeybindElement final : public UOptionData
{
	GENERATED_BODY()

	/*UPROPERTY()
	UInputMappingContext* InputMappingContext;
	UPROPERTY()
	FEnhancedActionKeyMapping EnhancedActionKeyMapping;*/

	UPROPERTY()
	UCategory* Category;

	UPROPERTY()
	FPlayerKeyMapping PlayerKeyMapping;

	UPROPERTY()
	FString keyBinding;

	UPROPERTY()
	FString keyStr;

	UPROPERTY()
	FKey key;

	UPROPERTY(EditAnywhere, Category = "Custom")
	FString textFormat = "<normal>{0}</>";

public:
	//FORCEINLINE UInputMappingContext* GetInputMappingContext() const { return InputMappingContext; }
	//FORCEINLINE FEnhancedActionKeyMapping GetEnhancedActionKeyMapping() const { return EnhancedActionKeyMapping; }
	FORCEINLINE FPlayerKeyMapping GetPlayerKeyMapping() const { return PlayerKeyMapping; }
	FORCEINLINE FKey GetKey() const { return key; }
	FORCEINLINE FString GetKeyStr() const { return keyStr; }
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const TObjectPtr<USettingsSave>& _settings) override;
	virtual TArray<FString> GetChoices() const override;

	void SetData(const FCustomKey& keyName, UCategory* _category, const FPlayerKeyMapping& _enhancedActionKeyMapping);

	void SetNewKey(const FKey& newKey);
};
