#include "NewStock.h"

#include "StockMenuSettings.h"
#include "Components/VerticalBoxSlot.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Members/MembersActor.h"
#include "RobotHunter/UI/Template/Hub/ButtonMemberPrefab.h"

void UNewStock::OpenOverlayLeftArm()
{
	ShowOverlay(true, EWeaponType::LEFT);
	UCharaAnim* _charaAnim = Cast<AMainCharacter>(GETFPC->GetPawn())->GetCharaAnim();
	CHECK_NULL(_charaAnim, "CharaAnim is null");
	_charaAnim->SetIsOpenLA(true);

	CHECK_NULL(parent, "StockActor is null");
	parent->GetCameraComponent()->UpdateCurrentSettings("LeftArm");
}

void UNewStock::OpenOverlayRightArm()
{
	ShowOverlay(true, EWeaponType::RIGHT);
	Cast<AMainCharacter>(GETFPC->GetPawn())->GetCharaAnim()->SetIsOpenRA(true);
	CHECK_NULL(parent, "StockActor is null");
	parent->GetCameraComponent()->UpdateCurrentSettings("RightArm");
}

void UNewStock::OpenOverlayThigh()
{
	ShowOverlay(true, EWeaponType::THIGH);
	Cast<AMainCharacter>(GETFPC->GetPawn())->GetCharaAnim()->SetIsOpenThigh(true);
	CHECK_NULL(parent, "StockActor is null");
	parent->GetCameraComponent()->UpdateCurrentSettings("Thigh");
}

void UNewStock::OpenOverlayCalf()
{
	ShowOverlay(true, EWeaponType::CALF);
	Cast<AMainCharacter>(GETFPC->GetPawn())->GetCharaAnim()->SetIsOpenCalf(true);
	CHECK_NULL(parent, "StockActor is null");
	parent->GetCameraComponent()->UpdateCurrentSettings("Calf");
}

void UNewStock::InitWithDelay()
{
	Super::InitWithDelay();
	ShowOverlay(false);
	LoadMembers();
}

void UNewStock::Back()
{
	Cast<AMainCharacter>(GETFPC->GetCharacter())->QuitMenu();
	if (media)
	{
		media->Pause();
		LOG_COLOR(true, "Pause Media", PINK);
	}
}

void UNewStock::RegisterEvents()
{
	Super::RegisterEvents();

	BIND_ACTION(ButtonLeftArm, OnClick, UNewStock, OpenOverlayLeftArm);
	BIND_ACTION(ButtonRightArm, OnClick, UNewStock, OpenOverlayRightArm);
	BIND_ACTION(ButtonThigh, OnClick, UNewStock, OpenOverlayThigh);
	BIND_ACTION(ButtonCalf, OnClick, UNewStock, OpenOverlayCalf);
	BIND_ACTION(backButton, OnClick, UNewStock, Back);
}

void UNewStock::Refresh()
{
	Super::Refresh();
	//ButtonRightArm->SetFocus();
}

void UNewStock::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	if (isGamepad)
		ButtonRightArm->SetFocus();
}


void UNewStock::LoadMembers()
{
	CHECK_NULL_WITH_RETURN(settings, "Settings is null", return)
	InitializeLeftArms();
	InitializeRightArms();
	InitializeLeftLegs();
	InitializeRightLegs();
}

void UNewStock::InitializeLeftArms()
{
	for (TSubclassOf Class : settings->GetLeftArms())
	{
		InitializeMemberInList(Class, LeftArm);
	}
}

void UNewStock::InitializeRightArms()
{
	for (TSubclassOf Class : settings->GetRightArms())
	{
		InitializeMemberInList(Class, RightArm);
	}
}

void UNewStock::InitializeLeftLegs()
{
	for (TSubclassOf Class : settings->GetCalfs())
	{
		InitializeMemberInList(Class, Calf);
	}
}

void UNewStock::InitializeRightLegs()
{
	for (TSubclassOf Class : settings->GetThighs())
	{
		InitializeMemberInList(Class, Thigh);
	}
}

void UNewStock::InitializeMemberInList(const TSubclassOf<AMembersActor>& _actor, const TObjectPtr<UVerticalBox>& _list)
{
	UButtonMemberPrefab* _button = CreateWidget<UButtonMemberPrefab>(GetWorld(), settings->GetTemplateButton());
	CHECK_NULL(_button, "Button widget could not be created");
	_list->AddChild(_button);
	UVerticalBoxSlot* _slot = Cast<UVerticalBoxSlot>(_button->Slot);
	_slot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	_button->SetMember(_actor, this);
}

void UNewStock::ShowInformation(EWeaponType _weaponType)
{
	if (!temporary)
	{
		AMainCharacter* _pc = Cast<AMainCharacter>(GETFPC->GetCharacter());
		CHECK_NULL(_pc, "pc is null");
		switch (_weaponType)
		{
		case EWeaponType::LEFT:
			temporary = _pc->GetUsingLeftArm();
			break;
		case EWeaponType::RIGHT:
			temporary = _pc->GetUsingRightArm();
			break;
		case EWeaponType::CALF:
			temporary = _pc->GetUsingCalf();
			break;
		case EWeaponType::THIGH:
			temporary = _pc->GetUsingThigh();
			break;
		default: break;
		}
	}
	if (!temporary)
	{
		return;
	}

	TObjectPtr<UWeaponInformationWidget> _information = isLeft ? RightInformation : LeftInformation;
	_information->SetVisibility(ESlateVisibility::Visible);

	CHECK_NULL(_information, "Information widget could not be created");

	_information->InitWidget(temporary->GetMemberName(), temporary->GetMaterialVideo(),
	                         temporary->GetMemberDescription());

	if (media)
		media->Close();
	media = temporary->GetMediaPlayerMember();
	CHECK_NULL(media, "Media widget could not be created");
	media->OpenSource(temporary->GetVideoOfMember());
	media->Rewind();
	media->SetRate(1.0f);
}

void UNewStock::SetMemberButtonSelected(EWeaponType _weaponType, const TObjectPtr<UButtonMemberPrefab>& _member)
{
	if (!buttonMember.Contains(_weaponType))
	{
		buttonMember.Add(_weaponType, _member);
	}
	else
	{
		buttonMember[_weaponType] = _member;
	}
}

void UNewStock::CloseOverlay()
{
	ShowOverlay(false);
	switch (overlayType)
	{
	case EWeaponType::LEFT:
		ButtonLeftArm->SetFocus();
		break;
	case EWeaponType::RIGHT:
		ButtonRightArm->SetFocus();
		break;
	case EWeaponType::CALF:
		ButtonCalf->SetFocus();
		break;
	case EWeaponType::THIGH:
		ButtonThigh->SetFocus();
		break;
	default: break;
	}
}

void UNewStock::CloseOverlayFromUI()
{
	onCloseOverlay.Broadcast();
	ShowOverlay(false);
	Cast<AMainCharacter>(GETFPC->GetCharacter())->SetInSubMenu(false);
}

void UNewStock::LoadWeaponInformation(const TObjectPtr<UVerticalBox>& Object)
{
	for (UWidget* AllChild : Object->GetAllChildren())
		if (UButtonMemberPrefab* _prefab = Cast<UButtonMemberPrefab>(AllChild))
			if (_prefab->IsEquipped())
				_prefab->Callback_OnHover();
}

void UNewStock::ShowOverlay(const bool _visible, EWeaponType _weaponType)
{
	HideAll();
	backButton->SetVisibility(_visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	if (!_visible)
	{
		ButtonLeftArm->SetVisibility(ESlateVisibility::Visible);
		ButtonRightArm->SetVisibility(ESlateVisibility::Visible);
		ButtonCalf->SetVisibility(ESlateVisibility::Visible);
		ButtonThigh->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetPawn());
	_chara->SetInSubMenu(true);
	overlayType = _weaponType;

	isLeft = _weaponType == EWeaponType::LEFT || _weaponType == EWeaponType::CALF;

	overlay->SetVisibility(ESlateVisibility::Visible);

	RightInformation->SetVisibility(ESlateVisibility::Hidden);
	LeftInformation->SetVisibility(ESlateVisibility::Hidden);
	ButtonLeftArm->SetVisibility(ESlateVisibility::Hidden);
	ButtonRightArm->SetVisibility(ESlateVisibility::Hidden);
	ButtonCalf->SetVisibility(ESlateVisibility::Hidden);
	ButtonThigh->SetVisibility(ESlateVisibility::Hidden);


	int index = 0;
	switch (_weaponType)
	{
	case EWeaponType::LEFT:
		//HBLeft->SetVisibility(ESlateVisibility::Visible);
		LeftArm->SetVisibility(ESlateVisibility::Visible);
		LoadWeaponInformation(LeftArm);
		RightInformation->SetVisibility(ESlateVisibility::Visible);
		//ButtonLeftArm->SetVisibility(ESlateVisibility::Visible);
		for (UWidget* AllChild : LeftArm->GetAllChildren())
		{
			if (UButtonMemberPrefab* _member = Cast<UButtonMemberPrefab>(AllChild))
			{
				if (_member->IsEquipped())
					break;
			}
			index++;
		}
		LeftArm->GetChildAt(index)->SetFocus();
		break;

	case EWeaponType::RIGHT:
		//HBRight->SetVisibility(ESlateVisibility::Visible);
		RightArm->SetVisibility(ESlateVisibility::Visible);
		LoadWeaponInformation(RightArm);
		LeftInformation->SetVisibility(ESlateVisibility::Visible);
		//ButtonRightArm->SetVisibility(ESlateVisibility::Visible);
		for (UWidget* AllChild : RightArm->GetAllChildren())
		{
			if (UButtonMemberPrefab* _member = Cast<UButtonMemberPrefab>(AllChild))
			{
				if (_member->IsEquipped())
					break;
			}
			index++;
		}
		RightArm->GetChildAt(index)->SetFocus();
		break;

	case EWeaponType::CALF:
		//HBLeft->SetVisibility(ESlateVisibility::Visible);
		Calf->SetVisibility(ESlateVisibility::Visible);
		LoadWeaponInformation(Calf);
		RightInformation->SetVisibility(ESlateVisibility::Visible);
		//ButtonCalf->SetVisibility(ESlateVisibility::Visible);
		for (UWidget* AllChild : Calf->GetAllChildren())
		{
			if (UButtonMemberPrefab* _member = Cast<UButtonMemberPrefab>(AllChild))
			{
				if (_member->IsEquipped())
					break;
			}
			index++;
		}
		Calf->GetChildAt(index)->SetFocus();
		break;

	case EWeaponType::THIGH:
		//HBRight->SetVisibility(ESlateVisibility::Visible);
		Thigh->SetVisibility(ESlateVisibility::Visible);
		LoadWeaponInformation(Thigh);
		LeftInformation->SetVisibility(ESlateVisibility::Visible);
		//ButtonThigh->SetVisibility(ESlateVisibility::Visible);
		for (UWidget* AllChild : Thigh->GetAllChildren())
		{
			if (UButtonMemberPrefab* _member = Cast<UButtonMemberPrefab>(AllChild))
			{
				if (_member->IsEquipped())
					break;
			}
			index++;
		}
		Thigh->GetChildAt(index)->SetFocus();
		break;

	default:
		break;
	}

	ShowInformation(_weaponType);
}

void UNewStock::HideAll() const
{
	overlay->SetVisibility(ESlateVisibility::Hidden);
	//HBLeft->SetVisibility(ESlateVisibility::Hidden);
	//HBRight->SetVisibility(ESlateVisibility::Hidden);
	LeftArm->SetVisibility(ESlateVisibility::Hidden);
	RightArm->SetVisibility(ESlateVisibility::Hidden);
	Calf->SetVisibility(ESlateVisibility::Hidden);
	Thigh->SetVisibility(ESlateVisibility::Hidden);

	UCharaAnim* _charaAnim = Cast<AMainCharacter>(GETFPC->GetPawn())->GetCharaAnim();
	CHECK_NULL(_charaAnim, "CharaAnim is null");
	_charaAnim->SetIsOpenLA(false);
	_charaAnim->SetIsOpenRA(false);
	_charaAnim->SetIsOpenThigh(false);
	_charaAnim->SetIsOpenCalf(false);

	CHECK_NULL(parent, "StockActor is null");
	parent->GetCameraComponent()->UpdateCurrentSettings("Idle");
}
