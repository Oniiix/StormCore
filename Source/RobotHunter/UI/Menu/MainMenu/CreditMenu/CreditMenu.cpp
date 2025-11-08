// 


#include "CreditMenu.h"

#include "Components/VerticalBoxSlot.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/UI/HUD/MainHUD.h"

void UCreditMenu::NativePreConstruct()
{
	Super::NativePreConstruct();

	vbNames->ClearChildren();
	CHECK_NULL(titleSectionReference, "Title section refernce is null");
	CHECK_NULL(namePrefabReference, "name prefab refernce is null");

	for (const FTypeSection& Section : sections)
	{
		USeparatorPrefab* _separator = CreateWidget<USeparatorPrefab>(this, titleSectionReference);
		CHECK_NULL(_separator, "Separator is null");
		vbNames->AddChildToVerticalBox(_separator);
		_separator->SetCategoryName(Section.nameSection.ToString());

		for (const FString& Name : Section.names)
		{
			UPeopleNamePrefab* _namePrefab = CreateWidget<UPeopleNamePrefab>(this, namePrefabReference);
			CHECK_NULL(_namePrefab, "NamePrefab is null");
			vbNames->AddChildToVerticalBox(_namePrefab);
			_namePrefab->SetNamePeople(FORMAT(formatName, Name));
			UVerticalBoxSlot* _slot = Cast<UVerticalBoxSlot>(_namePrefab->Slot);
			CHECK_NULL(_slot, "SLot is null");
			_slot->SetHorizontalAlignment(HAlign_Center);
			_slot->SetVerticalAlignment(VAlign_Center);
		}

		USpacer* _spacer = NewObject<USpacer>(this, spacerReference);
		CHECK_NULL(_spacer, "Separator is null");
		vbNames->AddChildToVerticalBox(_spacer);
		_spacer->SetSize(FVector2D(0, sizeSpacer));
	}
}

void UCreditMenu::Init()
{
	Super::Init();
	isInGame = GetWorld()->GetName() != "P_MainMenu";
	//LOG_DEBUG("IsInGame ? : " + INTSTR(isInGame));
}

void UCreditMenu::Refresh()
{
	Super::Refresh();
	onRefresh.Broadcast();
	backButton->SetIsEnabled(true);
	
}

void UCreditMenu::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(backButton, OnClick, UCreditMenu, Back);
}

void UCreditMenu::Back()
{
	onBack.Broadcast();
	if (!isInGame)
		hud->SetState(EStateUI::MainMain);
	else
	{
		USaveManagerSubsystem* _saveManager = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
		_saveManager->DeleteSave(_saveManager->GetIndexOfSave());
		UGameplayStatics::OpenLevel(GetWorld(), FName("P_MainMenu"));
	}
}
