// 


#include "UBonusATH.h"
#include "CardTemplate.h"
#include "Components/HorizontalBox.h"
#include "RobotHunter/UI/HUD/GameHUD.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"

void UBonusAth::Init()
{
	Super::Init();
	SetVisibility(enableDebug ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	for (UWidget* AllChild : HBBonus->GetAllChildren())
	{
		if (UCardTemplate* _card = Cast<UCardTemplate>(AllChild))
		{
			_card->SetParent(this);
		}
	}
	validBonus->SetVisibility(ESlateVisibility::Hidden);
	for (UWidget* Child : HBBonus->GetAllChildren())
	{
		if (UCardTemplate* _card = Cast<UCardTemplate>(Child))
		{
			MainWidget = _card;
			break;
		}
	}
}

void UBonusAth::InitWithDelay()
{
	Super::InitWithDelay();

	if (AGameHUD* _hud = Cast<AGameHUD>(GETFPC->GetHUD()))
		Setup(_hud);
}

void UBonusAth::RegisterEvents()
{
	Super::RegisterEvents();
	if (chara = Cast<AMainCharacter>(GETFPC->GetCharacter()))
		BIND_ACTION(chara, OnChooseBonus, UBonusAth, RegisterBonus);

	BIND_ACTION(validBonus, OnClick, UBonusAth, ApplyBonus);
}

void UBonusAth::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	if (isGamepad)
		MainWidget->SetFocus();

	if (hud->GetCurrentUI<UBonusAth>() == this)
		GETFPC->SetShowMouseCursor(!isGamepad);
}

void UBonusAth::RegisterBonus(TArray<FBonusStruct> _bonus)
{
	bonus = _bonus;
	int index = 0;
	if (CardSelected)
	{
		CardSelected->UnSelectCard();
		CardSelected = nullptr;
	}
	for (UWidget* AllChild : HBBonus->GetAllChildren())
	{
		if (UCardTemplate* _card = Cast<UCardTemplate>(AllChild))
		{
			_card->SetBonus(_bonus[index]);
			index++;
		}
	}
	chara->ResetChara();
	chara->SetDisableInput(true);
	hud->GetFocusSystem()->SetFocusTo(EFocusMode::UI);
	LaunchAnimationShow();
}

void UBonusAth::LaunchAnimationShow() const
{
	OnShow.Broadcast();
	MainWidget->SetFocus();
}

void UBonusAth::LaunchAnimationHide() const
{
	OnHide.Broadcast();
}

void UBonusAth::ReattributeInput() const
{
	chara->ReactivateMembers({});
	chara->SetDisableInput(false);
	hud->GetFocusSystem()->SetFocusTo(EFocusMode::GAME);
}

void UBonusAth::SetCardSelected(UCardTemplate* CardTemplate)
{
	OnCardSwitch.Broadcast(CardSelected, CardTemplate);
	CardSelected = CardTemplate;
	validBonus->SetVisibility(ESlateVisibility::Visible);
}

void UBonusAth::ApplyBonus()
{
	validBonus->SetVisibility(ESlateVisibility::Hidden);
	if (AMainCharacter* _character = Cast<AMainCharacter>(GETFPC->GetCharacter()))
	{
		_character->GetBonusComponent()->AddBonus(CardSelected->GetBonusType());
		OnApplyBonus.Broadcast(CardSelected->GetBonusType());
	}
	else
	{
		LOG_ERROR(true, "Failed to get character to apply bonus");
	}
}
