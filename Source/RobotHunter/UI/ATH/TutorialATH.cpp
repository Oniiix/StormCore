// 


#include "TutorialATH.h"

#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/HUD/GameHUD.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"
#include "RobotHunter/UI/Tuto/UI/TutoHint.h"
#include "RobotHunter/UI/Tuto/UI/DataAsset/TutorialHintAsset.h"
#include "Tutorial/TutorialPopup.h"

void UTutorialATH::Init()
{
	Super::Init();
	for (UWidget* AllChild : verticalBox->GetAllChildren())
		AllChild->RemoveFromParent();
	tutorialPopup->SetVisibility(ESlateVisibility::Collapsed);
	verticalBox->SetVisibility(ESlateVisibility::Visible);

	if (AGameHUD* _hud = Cast<AGameHUD>(GETFPC->GetHUD()))
		hud = _hud;
}

void UTutorialATH::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(tutorialPopup->GetValidateButton(), OnClick, UTutorialATH, CloseTutorialPopup);
}

void UTutorialATH::ResetTutorial()
{
	if (tutorialHint && currentInputCondition)
	{
		currentInputCondition->CancelCondition();
		currentInputCondition->OnValidateCondition().Clear();
		tutorialHint->RemoveFromParent();
		tutorialHint = nullptr;
	}
}

void UTutorialATH::AddTutorial(UTutorialHintAsset* _asset, const TObjectPtr<UConditionDataAsset>& _inputCondition)
{
	ResetTutorial();
	currentInputCondition = _inputCondition;

	CHECK_NULL(tutorialHintReference, "Reference hint is null");
	tutorialHint = CreateWidget<UTutoHint>(this, tutorialHintReference);
	CHECK_NULL(tutorialHint, "Popup is null");

	verticalBox->AddChildToVerticalBox(tutorialHint);

	BIND_ACTION(_asset, OnValidateProgress, UTutorialATH, UpdateProgress);
	tutorialHint->Setup(_asset);
	tutorialHint->Callback_OnShow();
}

void UTutorialATH::ShowBigTutorial(const UTutorialPopupAsset* TutorialAsset)
{
	tutorialPopup->SetVisibility(ESlateVisibility::Visible);
	tutorialPopup->Setup(TutorialAsset, this);
	verticalBox->SetVisibility(ESlateVisibility::Hidden);
	if (AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter()))
	{
		_chara->ResetChara();
		_chara->SetDisableInput(true);
		Cast<AMainHUD>(GETFPC->GetHUD())->GetFocusSystem()->SetFocusTo(EFocusMode::UI, _chara, false, false);
	}
}

void UTutorialATH::CloseTutorialPopup()
{
	tutorialPopup->SetVisibility(ESlateVisibility::Collapsed);
	tutorialPopup->Stop();
	if (AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter()))
	{
		_chara->SetDisableInput(false);
		_chara->ReactivateMembers({});
		Cast<AMainHUD>(GETFPC->GetHUD())->GetFocusSystem()->SetFocusTo(EFocusMode::GAME, _chara, false, false);
	}
	verticalBox->SetVisibility(ESlateVisibility::Visible);
}

void UTutorialATH::UpdateProgress(float Progress)
{
	CHECK_NULL(tutorialHint, "Tutorial hint is null");
	tutorialHint->SetValue(Progress);
	if (Progress >= 1)
	{
		tutorialHint->Callback_OnHide();
	}
}
