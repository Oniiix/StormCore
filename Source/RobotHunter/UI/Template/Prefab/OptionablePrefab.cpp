#include "OptionablePrefab.h"

#include "RobotHunter/UI/Menu/Options/Data/OptionData.h"


void UOptionablePrefab::SetOptionData(const TObjectPtr<UOptionData>& OptionData)
{
	UFileLogger::WriteLog(ELogSeverity::INFO, ELogCategory::UI, GetClass(), "Set option data");
	data = OptionData;
	data->SetPrefab(this);
	index = data->GetCurrentValue();
	SetOptionParameter(data->GetNameOption(), data->GetChoices());
	onChangeIndex.AddUniqueDynamic(data, &UOptionData::ChangeValue);
	onChangeIndex.AddUniqueDynamic(this, &UOptionablePrefab::SetChangeDetected);
}

void UOptionablePrefab::SetChangeDetected(const int& I)
{
	data->SetHasChanged(true);
}

void UOptionablePrefab::SetOptionParameter(const FString& _name, const TArray<FString>& _list)
{
}
