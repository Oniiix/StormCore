// 


#include "CooldownWeapon.h"

#include "RobotHunter/InputSystem/InputSubsystem.h"

void UCooldownWeapon::Init()
{
	Super::Init();
	subsystem = GetGameInstance()->GetSubsystem<UInputSubsystem>();
}

void UCooldownWeapon::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(subsystem, OnInputControllerChange, UCooldownWeapon, HandleInputChanged);
	BIND_ACTION(subsystem, OnLoadKey, UCooldownWeapon, RefreshInput);
}

void UCooldownWeapon::HandleInputChanged(bool isGamepad)
{
	Super::HandleInputChanged(isGamepad);
	RefreshInput(isGamepad);
	/*if (isGamepad)	
		input->SetKeySelectionText(TEXTSTR(subsystem->FindKey(member->GetInputMappingNameGamepad()).ToString()));
	else
		input->SetKeySelectionText(TEXTSTR(subsystem->FindKey(member->GetInputMappingName()).ToString()));*/
}

void UCooldownWeapon::SetupKey(const FKey& _key)
{
	CHECK_NULL(subsystem->GetKeyImage(), "keyAsset in CooldownWeapon is null!");
	TMap<FKey, UMaterialInterface*> _keyMap = subsystem->GetKeyImage()->GetkeyMap();


	if (!_keyMap.Contains(_key))
	{
		input->SetSelectedKey(_key);
		input->SetVisibility(ESlateVisibility::Visible);
		imageInput->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	imageInput->SetBrushFromMaterial(_keyMap[_key]);
	imageInput->SetVisibility(ESlateVisibility::Visible);
	input->SetVisibility(ESlateVisibility::Collapsed);
}

void UCooldownWeapon::SetupMember(AMembersActor* _member)
{
	member = _member;

	imageWeapon->SetBrushFromMaterial(_member->GetIcon());

	if (subsystem->FindKey(member->GetInputMappingName()).GetFName() != "None")
		RefreshInput();
}

void UCooldownWeapon::RefreshInput(const bool _isGamepad)
{
	if (!input || !subsystem || !member || member && member->GetInputMappingName() == "") return;

	const FKey key = subsystem->FindKey(member->GetInputMappingName(_isGamepad));


	SetupKey(key);
}

void UCooldownWeapon::Use(const float _cooldown) const
{
	//LOG_DEBUG("Call use event");
	onUseWeapon.Broadcast(_cooldown);
}
