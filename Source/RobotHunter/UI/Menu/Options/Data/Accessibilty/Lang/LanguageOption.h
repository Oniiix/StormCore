#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "../../../../../../UI/Template/Popup/Asset/PopupData.h"
#include "LanguageOption.generated.h"

UENUM()
enum class EGameLanguage
{
	English = 0 UMETA(DisplayName = "English"),
	French UMETA(DisplayName = "Français"),

	None UMETA(DisplayName = "Choose Option..."),
};

USTRUCT()
struct FLanguage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText langName;

	UPROPERTY(EditAnywhere)
	FString shortcut;

	FLanguage()
	{
	}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTHUNTER_API ULanguageOption final : public UOptionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Language")
	TMap<EGameLanguage, FLanguage> LanguageOptions;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPopupData> restartData;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;
};
