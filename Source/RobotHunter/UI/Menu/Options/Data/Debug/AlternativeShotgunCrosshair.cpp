// 


#include "AlternativeShotgunCrosshair.h"

#include "RobotHunter/Game/CustomGameMode.h"

void UAlternativeShotgunCrosshair::Init(const TObjectPtr<UOptionFrame>& _parent,
                                        const TObjectPtr<UGameUserSettings>& _gus, const TObjectPtr<UWorld>& _world,
                                        const TObjectPtr<USettingsSave>& _settings)
{
	Super::Init(_parent, _gus, _world, _settings);
	currentIndex = static_cast<int>(settings->UseAlternativeShotgunCrosshair());
	//Apply();
}

void UAlternativeShotgunCrosshair::Apply()
{
	Super::Apply();
	settings->SetUseAlternativeShotgunCrosshair(static_cast<bool>(currentIndex));
}

TArray<FString> UAlternativeShotgunCrosshair::GetChoices() const
{
	TArray<FString> result;

	for (const TTuple<bool, FText>& Value : values)
	{
		result.Add(Value.Value.ToString());
	}

	return result;
}

#if WITH_EDITOR
void UAlternativeShotgunCrosshair::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.ChangeType == EPropertyChangeType::ArrayRemove || PropertyChangedEvent.ChangeType ==
		EPropertyChangeType::ArrayClear)
	{
		values = INIT_LIST;
	}
}
#endif WITH_EDITOR
