// 


#include "QuestATH.h"

#include "Components/VerticalBoxSlot.h"
#include "RobotHunter/Game/CustomGameInstance.h"
#include "Template/CategoryQuestTemplate.h"
#include "Template/QuestDropdownTemplate.h"


void UQuestATH::AddQuest(UQuestAsset* QuestAsset)
{
	CHECK_NULL(QuestAsset, "QuestAsset is null");

	if (UVerticalBox* VB = QuestVB)
	{
		LOG_DEBUG("VB is valid");
		if (VB->GetChildrenCount() > 0 && !INSTANCEOF(VB->GetChildAt(0), UCategoryQuestTemplate))
		{
			VB->ClearChildren();
			slots.Empty();
		}
		if (VB->GetChildrenCount() == 0)
		{
			UCategoryQuestTemplate* questCategory = CreateWidget<UCategoryQuestTemplate>(this, questCategoryTemplate);
			CHECK_NULL(questCategory, "Quest Category template is null");
			VB->AddChildToVerticalBox(questCategory);

			if (UVerticalBoxSlot* slot = Cast<UVerticalBoxSlot>(questCategory->Slot))
				slot->SetSize(ESlateSizeRule::Automatic);

			const FText name = MainCategoryName;
			questCategory->InitializeCategory(VB, QuestAsset, this);
		}

		CreateQuestWidget(VB, QuestAsset);
	}
}

void UQuestATH::RecalculateSlot()
{
	if (!QuestVB) return;

	TArray<TObjectPtr<UQuestAsset>> Keys;
	slots.GetKeys(Keys);

	TMap<TObjectPtr<UQuestAsset>, int> NewSlots;

	for (int i = 1; i < QuestVB->GetChildrenCount(); i++)
	{
		if (Cast<UQuestDropdownTemplate>(QuestVB->GetChildAt(i)))
			NewSlots.Add(Keys[i - 1], i);
	}
	slots = NewSlots;
}

void UQuestATH::RemoveQuest(UQuestAsset* QuestAsset)
{
	if (!QuestVB || !slots.Contains(QuestAsset)) return;

	const int QuestIndex = slots[QuestAsset];

	if (QuestIndex > 0 && QuestIndex < QuestVB->GetChildrenCount())
	{
		if (Cast<UQuestDropdownTemplate>(QuestVB->GetChildAt(QuestIndex)))
		{
			//QuestWidget->PlayHideAnimation();

			QuestVB->RemoveChildAt(QuestIndex);
		}
	}

	slots.Remove(QuestAsset);

	RecalculateSlot();
}

int UQuestATH::GetPositionSlotOfQuest(const UQuestAsset* Asset)
{
	if (slots.Contains(Asset))
	{
		return slots[Asset];
	}
	return -1;
}

void UQuestATH::CreateQuestWidget(UVerticalBox* _VB, UQuestAsset* _questAsset)
{
	CHECK_NULL(questDropDownTemplate, "Quest Dropdown reference is null");
	UQuestDropdownTemplate* questWidget = CreateWidget<UQuestDropdownTemplate>(this, questDropDownTemplate);
	CHECK_NULL(questWidget, "Quest widget template is null");

	_VB->AddChildToVerticalBox(questWidget);

	const int index = _VB->GetChildIndex(questWidget);

	if (UVerticalBoxSlot* questSlot = Cast<UVerticalBoxSlot>(questWidget->Slot))
		questSlot->SetSize(ESlateSizeRule::Automatic);

	questWidget->InitializeQuest(_questAsset, FORMAT(questFormatText, _questAsset->GetTextQuest()), questFormatText,
	                             _VB, index, this);

	slots.Add(_questAsset, index);
}

void UQuestATH::RegisterEvents()
{
	Super::RegisterEvents();

	questsManager = GetGameInstance<UCustomGameInstance>()->GetQuestManager();
	CHECK_NULL(questsManager, "QUESTS MANAGER IS NULL IN QUEST ATH")
	questsManager->onQuestAdded.AddUniqueDynamic(this, &UQuestATH::AddQuest);
	questsManager->onQuestValidate.AddUniqueDynamic(this, &UQuestATH::ValidateQuest);
}

void UQuestATH::ValidateQuest(const UQuestAsset* QuestAsset)
{
	CHECK_NULL(QuestAsset, "Quest asset is null");
	CHECK_NULL(QuestVB, "QuestVB is null");

	if (!slots.Contains(QuestAsset))
	{
		LOG_ERROR(true, "Quest asset not found in slots");
		return;
	}

	const int QuestIndex = slots[QuestAsset];

	if (QuestIndex < 0 || QuestIndex >= QuestVB->GetChildrenCount())
	{
		LOG_ERROR(true, "Quest index out of bounds: " + FString::FromInt(QuestIndex) +
		          " / " + FString::FromInt(QuestVB->GetChildrenCount()));
		return;
	}

	UWidget* ChildWidget = QuestVB->GetChildAt(QuestIndex);
	if (!ChildWidget)
	{
		LOG_ERROR(true, "Child widget at index is null");
		return;
	}
	if (UQuestDropdownTemplate* QuestWidget = Cast<UQuestDropdownTemplate>(ChildWidget))
	{
		LOG_DEBUG("Validating quest successfully");
		QuestWidget->PlayValidAnimation();
	}
	else
	{
		LOG_ERROR(true, "Widget at index " + FString::FromInt(QuestIndex) +
		          " is not a UQuestDropdownTemplate. Actual type: " +
		          (ChildWidget ? ChildWidget->GetClass()->GetName() : "Unknown"));
	}
}

void UQuestATH::Init()
{
	Super::Init();
	QuestVB->ClearChildren();
}

void UQuestATH::RefreshQuest()
{
	CHECK_NULL(questsManager, "QUESTS MANAGER IS NULL IN QUEST ATH")
	TArray<UQuestAsset*> _quests = questsManager->GetOnGoingQuests();
	for (UQuestAsset* quest : _quests)
	{
		AddQuest(quest);
	}
}

void UQuestATH::InitWithDelay()
{
	Super::InitWithDelay();
	RefreshQuest();
}
