// 


#include "QuestAsset.h"


#include "RobotHunter/UI/ATH/Game/Quest/Template/QuestDropdownTemplate.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#endif


FString UQuestAsset::GetTextQuest()
{
	TArray<FFormatArgumentValue> FormatArgs;
	for (const int& Arg : Args)
		FormatArgs.Add(FText::FromString(FString::FromInt(Arg)));

	const FText FormattedText = FText::Format(
		FText::FromString(text.ToString()),
		FormatArgs
	);

	return FormattedText.ToString();
}

void UQuestAsset::SetArgsParam(int argNumber, const FText _parameter)
{
	TArray<FFormatArgumentValue> FormatArgs;
	FormatArgs.Reserve(Args.Num());

	for (int32 i = 0; i < Args.Num(); ++i)
	{
		// Remplace l’argument spécifié par `_parameter`z
		if (i == argNumber)
			FormatArgs.Add(_parameter);
		else
			FormatArgs.Add(Args[i]);
	}

	// Formatte le texte
	const FText FormattedText = FText::Format(text, FormatArgs);

	if (!Widget) return;
	// Applique le texte formaté au Widget
	SET_TEXT_STR(Widget->GetText()->GetText(), FORMAT(Widget->GetFormatText(), FormattedText.ToString()));
}

void UQuestAsset::SetWidget(UQuestDropdownTemplate* _widget)
{
	Widget = _widget;
}

void UQuestAsset::UpdateParms(int _value)
{		
	SetArgsParam(0, FText::FromString(FString::FromInt(Args[0] += _value)));
}

void UQuestAsset::ResetParams()
{
	SetArgsParam(0, FText::FromString(FString::FromInt(0)));
}

void UQuestAsset::ValidQuest()
{
	if (conditionQuests.Num() < 1)
		isValidate = true;
	else if (CompleteConditionQuests() == conditionQuests.Num())
		isValidate = true;
}

int UQuestAsset::CompleteConditionQuests()
{
	TArray<UQuestAsset*> _completConditionQuests;
	for (size_t i = 0; i < conditionQuests.Num(); i++)
	{
		if (conditionQuests[i]->GetIsValidate())
			_completConditionQuests.Add(conditionQuests[i]);
	}
	return _completConditionQuests.Num();
}


#if WITH_EDITOR

void UQuestAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);


	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FString StrLower = PropertyName.ToString().ToLower();


	auto checkArgsCount = [&]
	{
		int32 maxArg = -1;
		FRegexPattern regexPattern(TEXT("\\{(\\d+)\\}"));
		FRegexMatcher matcher(regexPattern, text.ToString());

		while (matcher.FindNext())
		{
			FString numberString = matcher.GetCaptureGroup(1);
			int32 argIndex = FCString::Atoi(*numberString);
			if (argIndex > maxArg)
				maxArg = argIndex;
		}
		return maxArg >= 0 ? maxArg + 1 : 0;
	};

	auto refreshList = [&]
	{
		int32 _count = checkArgsCount();

		if (Args.Num() > _count)
		{
			for (int i = Args.Num() - 1; i >= _count; i--)
				Args.RemoveAt(i);
		}

		else if (Args.Num() < _count)
		{
			for (int i = Args.Num(); i < _count; ++i)
				Args.Add(i);
		}
	};


	TMap<FString, TFunction<void()>> Events =
	{
		{
			"args", [&]
			{
				refreshList();
			}
		},
		{
			"text", [&]
			{
				refreshList();
			}
		}
	};


	if (Events.Contains(StrLower))
		Events[StrLower]();
	textShowed = GetTextQuest();
}

void UQuestAsset::AssignRandomUniqueValue()
{
	// Gather all assets of this type in project
	TSet<int32> UsedValues;

	// Utilisation du AssetRegistry pour récupérer tous les DataAssets du même type
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;

	FARFilter Filter;
	Filter.ClassNames.Add(StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;

	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	for (const FAssetData& Data : AssetDataList)
	{
		if (Data.AssetName == GetFName())
			continue;
		int32 Value = 0;
		if (Data.GetTagValue("UniqueRandomValue", Value))
			UsedValues.Add(Value);
	}

	int32 NewValue = 0;
	constexpr int32 MaxTry = 5000;
	for (int32 i = 0; i < MaxTry; ++i)
	{
		NewValue = FMath::RandRange(1, 999999);
		if (!UsedValues.Contains(NewValue))
			break;
	}
	Id = NewValue;
	MarkPackageDirty();
}
#endif
