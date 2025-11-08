// 


#include "QuestSubsystem.h"

#include "RobotHunter/SaveSystem/GameplaySave.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/Utils/FileLogger.h"

void UQuestSubsystem::AddQuest(UQuestAsset* _asset)
{
	//LOG_COLOR_TIME(true, "NIKOUMOUK TA MAMAN", CYAN,10);
	if (ExistQuest(_asset) || IsCompleteQuest(_asset)) return;
	//LOG_COLOR_TIME(true, "NIKOUMOUK", CYAN,10);
	quests.Add(_asset);
	onQuestAdded.Broadcast(_asset);
}


void UQuestSubsystem::RemoveQuest(UQuestAsset* _asset)
{
	if (!ExistQuest(_asset)) return;
	quests.Remove(_asset);
	onQuestRemoved.Broadcast(_asset);
}

bool UQuestSubsystem::ExistQuest(const UQuestAsset* _asset) const
{
	return quests.Contains(_asset);
}

UQuestAsset* UQuestSubsystem::GetQuestFromIndex(const int _index)
{
	return quests[_index];
}

void UQuestSubsystem::ValidQuest(UQuestAsset* _asset)
{
	CHECK_NULL(_asset, "Asset is not set to validate quest");
	LOG_COLOR_TIME(true, FString::SanitizeFloat(_asset->GetCurrentQuestWidgets().Num()), RED, 3);
	for (size_t i = 0; i < _asset->GetCurrentQuestWidgets().Num(); i++)
	{
		CHECK_NULL(_asset->GetCurrentQuestWidgets()[i], "Asset don't have any widget");
		_asset->GetCurrentQuestWidgets()[i]->Destroy();
	}
	questsComplete.Add(_asset->GetIdQuest());

	GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave()->SaveQuests(questsComplete);
	GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->SaveCurrent(false);

	onQuestValidate.Broadcast(_asset);
}

void UQuestSubsystem::ClearAllQuests()
{
	for (UQuestAsset* Quest : quests)
	{
		RemoveQuest(Quest);
	}
}

void UQuestSubsystem::ForceClearAllQuests()
{
	quests.Empty();
}
