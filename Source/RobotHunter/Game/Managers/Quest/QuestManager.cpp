#include "QuestManager.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include <RobotHunter/SaveSystem/SaveManagerSubsystem.h>
#include "RobotHunter/SaveSystem/GameplaySave.h"

// Sets default values
AQuestManager::AQuestManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	InitQuests();
}

void AQuestManager::InitQuests()
{
	if (CheckTuto())
		return;
	LoadQuests();
	GetGameInstance<UCustomGameInstance>()->SetQuestManager(this);
	
	SetupQuests();
}

bool AQuestManager::CheckTuto()
{
	if (GetGameInstance<UCustomGameInstance>()->GetGameStatus() == EGameStatus::HUB && GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave())
	{
		if (!GetGameInstance<UCustomGameInstance>()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave()->GetTutoSave().hasStartTuto)
			return true;
		else
			return false;
	}
	else
		return false;
}

void AQuestManager::SetupQuests()
{
	for (TPair<int, UQuestAsset*> _pair : quests)
	{
		if (CheckCompleteQuest(_pair.Key))
			continue;
		if (_pair.Value->GetUseWidget())
			InitQuestWidget(_pair.Value);
	}
}

void AQuestManager::LoadQuests()
{
	CHECK_NULL(GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave(), "Y A PAS DE SAVE DE QUETES")
		LOG_COLOR(true, "Quest Complete => " + FString::FromInt(GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave()->GetQuestSave().IdQuests.Num()), BLUE);
	SetQuestCompleteList(GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave()->GetQuestSave().IdQuests);

	const size_t _count = completeQuests.Num();
	for (size_t i = 0; i < _count; i++)
	{
		if (quests.Contains(completeQuests[i]))
			quests.Remove(completeQuests[i]);
	}
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuestManager::AddQuest(UQuestAsset* _asset)
{
	CHECK_NULL(_asset, "ASSET IS NULL TO ADD")
	if(CheckTuto())
		return;
	if (!quests.Contains(_asset->GetIdQuest()))
	{
		quests.Add(_asset->GetIdQuest(), _asset);
		SetupQuests();
		onQuestAdded.Broadcast(_asset);
	}
}

void AQuestManager::InitQuestWidget(UQuestAsset* _asset)
{
	if(CheckTuto())
		return;
	_asset->GetCurrentQuestWidgets().Empty();
	TArray<AActor*> _outActors;
	if (!_asset->GetSearchByTag())
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), _asset->GetQuestActor(), _outActors);
	else
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), _asset->GetQuestActorTag(), _outActors);
	if (_outActors.Num() < 1 || !_asset->GetQuestWidgetRef())return;
	for (size_t i = 0; i < _outActors.Num(); i++)
	{
		AQuestWidget* _widget = GetWorld()->SpawnActor<AQuestWidget>(_asset->GetQuestWidgetRef());
		CHECK_NULL(_widget, "Widget has not spawned");
		//if (!_asset->GetCurrentQuestWidgets().Contains(_widget))
		_asset->SetCurrentQuestWidgets(_widget);
		_widget->SetActorLocation(_outActors[i]->GetActorLocation() + _asset->GetQuestLocationOffset());
		_widget->AttachToActor(_outActors[i], FAttachmentTransformRules::KeepWorldTransform);
	}
}

bool AQuestManager::ValidQuest(UQuestAsset* _asset)
{
	CHECK_NULL_WITH_RETURN(_asset, "Asset is not set to validate quest", return false);

	UQuestAsset* _goodQuest = nullptr;

	if (quests.Contains(_asset->GetIdQuest()))
		_goodQuest = quests[_asset->GetIdQuest()];
	CHECK_NULL_WITH_RETURN(_goodQuest, "QUEST IS NOT INIT", return false);
	if (CheckCompleteQuest(_goodQuest->GetIdQuest()))
		return false;

	LOG_COLOR_TIME(false, "QUEST VALIDATE", GREEN, 3);
	ValidQuest_Internal(_goodQuest);
	return true;
}

void AQuestManager::ValidQuest_Internal(UQuestAsset* _asset)
{
	AQuestWidget* _widget = nullptr;
	const size_t _count = _asset->GetCurrentQuestWidgets().Num();
	for (size_t i = 0; i < _count; i++)
	{
		CHECK_NULL(_asset->GetCurrentQuestWidgets()[i], "Asset don't have any widget");
		_widget = _asset->GetCurrentQuestWidgets()[i];
		_asset->GetCurrentQuestWidgets().Remove(_widget);
		_widget->SetLifeSpan(0.1);
	}
	_asset->ValidQuest();
	//quests.Remove(_asset->GetIdQuest());
	completeQuests.Add(_asset->GetIdQuest());
	UGameplaySave* _save = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>()->GetCurrentSave();
	if(_save)
		_save->SaveQuests(completeQuests);
	LOG_COLOR(false, _asset->GetName() + " VALIDATE", GREEN);
	onQuestValidate.Broadcast(_asset);
}

bool AQuestManager::CheckCompleteQuest(int _questId)
{
	const size_t _count = completeQuests.Num();
	for (size_t i = 0; i < _count; i++)
	{
		if (_questId == completeQuests[i])
			return true;
	}
	return false;
}

TArray<UQuestAsset*> AQuestManager::GetOnGoingQuests()
{
	TArray<UQuestAsset*> _resultList;

	for (const TPair<int, UQuestAsset*> _pair : quests)
	{
		if (!completeQuests.Contains(_pair.Key))
			_resultList.Add(_pair.Value);
	}
	return _resultList;
}


void AQuestManager::AddToQuest()
{
	if (!addingQuest) return;

	quests.Add(addingQuest->GetIdQuest(), addingQuest);
}
