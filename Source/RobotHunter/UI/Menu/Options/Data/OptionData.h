#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RobotHunter/SaveSystem/SettingsSave.h"
#include "RobotHunter/Utils/FileLogger.h"
#include "OptionData.generated.h"

class UOptionablePrefab;
class UOptionFrame;

UENUM()
enum class EOptionType : int8
{
	None UMETA(DisplayName = "Choose an option"),
	Selector UMETA(DisplayName = "Selector"),
	Checker UMETA(DisplayName = "Checker"),
	Slider UMETA(DisplayName = "Slider"),
	Rebind UMETA(DisplayName = "Rebind"),
};


/**
 * 
 */
UCLASS(BlueprintType, Abstract)
class ROBOTHUNTER_API UOptionData : public UObject
{
	GENERATED_BODY()

protected:
	/*
		The type of the option, can be of type:
		- Selector: Allows you to select one of several options.
		- Slider: Allows you to modify a value from a slider.
	*/
	UPROPERTY(EditAnywhere, Category = "Options")
	EOptionType OptionType = EOptionType::None;

	UPROPERTY()
	TObjectPtr<UGameUserSettings> GUS = nullptr;

	UPROPERTY()
	TObjectPtr<USettingsSave> settings = nullptr;
	UPROPERTY()
	TObjectPtr<UWorld> world;


	UPROPERTY(EditAnywhere, Category = "Options|Prefab",
		meta = (EditCondition = "OptionType != EOptionType::None", EditConditionHides))
	TSubclassOf<UOptionablePrefab> prefab;

	UPROPERTY(EditAnywhere, Category = "Options",
		meta = (EditCondition = "OptionType != EOptionType::Rebind", EditConditionHides))
	FText nameOption;

	UPROPERTY(EditAnywhere, Category = "Options",
		meta = (EditCondition = "OptionType != EOptionType::Rebind", EditConditionHides))
	FText descriptionOption;

	UPROPERTY(EditAnywhere, Category = "Options")
	FString nameFormat;

	UPROPERTY(EditAnywhere, Category = "Options")
	FString descriptionFormat;

	UPROPERTY(EditAnywhere, Category = "Options")
	FString choiceFormat;

	UPROPERTY(EditAnywhere, Category = "Options")
	bool canBeModifyInGame = true;

	UPROPERTY()
	bool hasChanged = false;

	UPROPERTY()
	TObjectPtr<UOptionFrame> parent;

	UPROPERTY()
	TObjectPtr<UOptionablePrefab> currentPrefab;

	UPROPERTY()
	int defaultIndex = 0;
	UPROPERTY()
	int currentIndex = 0;

public:
	virtual void Apply();

	FORCEINLINE virtual bool CanBeModifyInGame() { return canBeModifyInGame; }
	FORCEINLINE virtual void SetDefaultValue(const int _value) { defaultIndex = _value; }
	FORCEINLINE virtual bool HasChanged() { return hasChanged; }
	FORCEINLINE virtual void SetHasChanged(const bool _value) { hasChanged = _value; }
	FORCEINLINE void Reset()
	{
		currentIndex = defaultIndex;
		UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Reset options");
	}

	FORCEINLINE FString GetNameOption() const { return FORMAT(nameFormat, nameOption.ToString()); }
	FORCEINLINE FString GetDescription() const { return FORMAT(descriptionFormat, descriptionOption.ToString()); }
	FORCEINLINE TSubclassOf<UOptionablePrefab> GetPrefab() const { return prefab; }
	FORCEINLINE TObjectPtr<UOptionablePrefab> GetCurrentPrefab() const { return currentPrefab; }
	FORCEINLINE EOptionType GetOptionType() const { return OptionType; }
	FORCEINLINE int GetDefaultIndex() const { return defaultIndex; }

	UFUNCTION()
	virtual void ChangeValue(const int& _value);

	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings);
	virtual int GetCurrentValue() const;

	virtual TArray<FString> GetChoices() const PURE_VIRTUAL(UOptionData::GetChoices, return {};)
	//virtual void  ResetToDefault() PURE_VIRTUAL(UOptionData::ResetToDefault, );
	virtual void  ResetToDefault();

	void SetPrefab(const TObjectPtr<UOptionablePrefab>& _currentPrefab);

protected:
	void ExecuteCommand(const FString& _cmd) const;
};
