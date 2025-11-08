#include "MenuSettings.h"
#include "Blueprint/UserWidget.h"
#include "RobotHunter/UI/Frame/BaseFrame.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"

bool UMenuSettings::LoadCurrentFrame()
{
	CHECK_NULL_WITH_RETURN(frameReference, "Frame reference is null in settings HUD", false);

	currentFrame = CreateWidget<UBaseFrame>(hud->GetWorld(), frameReference);

	CHECK_NULL_WITH_RETURN(currentFrame, "Current frame is null", false);

	currentFrame->Show();
	return true;
}

void UMenuSettings::LoadAllUIs()
{
	allUIs.Empty();
	for (const TTuple<EStateUI, TSubclassOf<UUI>>& _element : allUIsReference)
	{
		EStateUI _key = _element.Key;

		FString _keyValue = GET_ENUM_STRING(_key);
		if (allUIs.Contains(_key))
		{
			LOG_ERROR(true, "Key " + _keyValue + " already exists.");
			continue;
		}

		const TObjectPtr<UUI> _widget = CreateWidget<UUI>(hud->GetWorld(), _element.Value);
		CHECK_NULL_WITH_RETURN(_widget, "Widget at " + _keyValue + " is null", continue);

		allUIs.Add(_key, _widget);
	}

	for (const TTuple<EStateUI, TObjectPtr<UUI>>& AllUI : allUIs)
	{
		AllUI.Value->Setup(hud);
	}
}

void UMenuSettings::SetupSwitcher()
{
	CHECK_NULL(currentFrame, "Current frame is null");

	currentFrame->GetSwitcher()->ClearChildren();

	for (const TTuple<EStateUI, TObjectPtr<UUI>>& uiPair : allUIs)
	{
		currentFrame->GetSwitcher()->AddChild(uiPair.Value);
	}
}

void UMenuSettings::Setup(const TObjectPtr<AMainHUD>& _hud)
{
	Super::Setup(_hud);
	if (!LoadCurrentFrame())
	{
		return;
	}
	LoadAllUIs();
	SetupSwitcher();
	hud->SetState(startUI);
}

UBaseFrame* UMenuSettings::GetCurrentFrame() const
{
	return currentFrame;
}

int UMenuSettings::GetIndexFromState(const EStateUI _state) const
{
	if (!allUIs.Contains(_state))
	{
		return -1;
	}

	int index = 0;
	for (const TTuple<EStateUI, UUI*> uiPair : allUIs)
	{
		if (uiPair.Key == _state)
		{
			return index;
		}
		index++;
	}
	return -1;
}
