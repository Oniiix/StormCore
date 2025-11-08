// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "FullScreenOption.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, HideDropdown)
class ROBOTHUNTER_API UFullScreenOption final : public UOptionData
{
	GENERATED_BODY()

	/*Set the different state of type screen
	 */
	UPROPERTY(EditAnywhere, Category = "Custom", meta = (EditFixedSize))
	TMap<TEnumAsByte<EWindowMode::Type>, FText> TypeWindowMode =
	{
		{EWindowMode::Type::Fullscreen, TEXTSTR("FullScreen")},
		{EWindowMode::Type::WindowedFullscreen, TEXTSTR("Window FullScreen")},
		{EWindowMode::Type::Windowed, TEXTSTR("Windowed")}
	};

	UPROPERTY(EditAnywhere, Category = "Custom")
	TEnumAsByte<EWindowMode::Type> DefaultWindowMode = EWindowMode::Type::Fullscreen;

public:
	virtual void Init(const TObjectPtr<UOptionFrame>& _parent, const TObjectPtr<UGameUserSettings>& _gus,
	                  const TObjectPtr<UWorld>& _world, const
	                  TObjectPtr<USettingsSave>& _settings) override;
	virtual void Apply() override;
	virtual TArray<FString> GetChoices() const override;
	virtual void ResetToDefault() override;

private:
	int GetIndexFromTypeWindow(EWindowMode::Type Window) const;
	EWindowMode::Type GetWindowFromIndex(int Index) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

inline EWindowMode::Type UFullScreenOption::GetWindowFromIndex(int Index) const
{
	TArray<TEnumAsByte<EWindowMode::Type>> _arrays;
	TypeWindowMode.GenerateKeyArray(_arrays);
	return _arrays[Index];
}
