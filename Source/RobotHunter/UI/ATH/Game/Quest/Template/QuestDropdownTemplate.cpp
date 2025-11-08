// 


#include "QuestDropdownTemplate.h"

#include "RobotHunter/UI/ATH/Game/Quest/QuestATH.h"

void UQuestDropdownTemplate::NativeConstruct()
{
	Super::NativeConstruct();
	SET_TEXT_STR(textTitle->GetText(), text);
}

void UQuestDropdownTemplate::InitializeQuest(UQuestAsset* _quest, const FString& _name, const FString& _formatQuestText,
                                             const TObjectPtr<UVerticalBox>& _questVB,
                                             const int _indexVB, UQuestATH* _ath)
{
	questAsset = _quest;
	text = _name;
	onInitQuest.Broadcast();
	indexVB = _indexVB;
	vbQuest = _questVB;
	_quest->SetWidget(this);
	formatText = _formatQuestText;
	ath = _ath;
}

void UQuestDropdownTemplate::PlayHideAnimation() const
{
	onHideAnimation.Broadcast();
}

void UQuestDropdownTemplate::RemoveWidgetFromList() const
{
	ath->RemoveQuest(questAsset);
}

void UQuestDropdownTemplate::PlayValidAnimation() const
{
	onValidate.Broadcast();
}
