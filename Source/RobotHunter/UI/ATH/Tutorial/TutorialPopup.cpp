// 


#include "TutorialPopup.h"

#include "TutorialPopupAsset.h"
#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "RobotHunter/UI/Template/Prefab/ButtonPrefab/ButtonPrefab.h"

class UInputSubsystem;

void UTutorialPopup::Init()
{
	Super::Init();

	SetIsFocusable(true);
	buttonValidate->SetFocus();
}


void UTutorialPopup::Setup(const UTutorialPopupAsset* Asset, UTutorialATH* TutorialAth)
{
	currentAsset = Asset;
	tutorialATH = TutorialAth;
	Asset->GetMediaPlayer()->OpenSource(Asset->GetMediaSource());
	Asset->GetMediaPlayer()->Rewind();

	media->SetBrushFromMaterial(Asset->GetMediaBrush());

	Asset->GetMediaPlayer()->Play();

	SET_TEXT_STR(text->GetText(), Asset->GetFormatString());

	onShow.Broadcast();
}

void UTutorialPopup::Stop() const
{
	currentAsset->GetMediaPlayer()->Close();
	media->SetBrushFromMaterial(nullptr);

	onQuit.Broadcast();
}


void UTutorialPopup::RegisterEvents()
{
	Super::RegisterEvents();
	BIND_ACTION(GetGameInstance()->GetSubsystem<UInputSubsystem>(), OnInputControllerChange, UTutorialPopup,
	            OnInputChange);
}

void UTutorialPopup::OnInputChange(bool _isGamepad)
{
	if (_isGamepad)
		buttonValidate->SetFocus();
}
