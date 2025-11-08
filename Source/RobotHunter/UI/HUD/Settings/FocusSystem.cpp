#include "FocusSystem.h"
#include "Kismet/GameplayStatics.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/UI/HUD/MainHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"

void UFocusSystem::SetFocusTo(const EFocusMode _inputMode,AMainCharacter* _mainCharacter, bool _bQuitMenu, bool _isPause) const
{
	if (!hud)
	{
		return;
	}

	const TObjectPtr<APlayerController> _fpc = hud->GetWorld()->GetFirstPlayerController();

	CHECK_NULL(_fpc, "FPC is null");

	if (_mainCharacter == nullptr)
		_mainCharacter = Cast<AMainCharacter>(_fpc->GetPawn());
	EMouseCaptureMode mode = EMouseCaptureMode::CapturePermanently;
	switch (_inputMode)
	{
	case EFocusMode::GAME:
		mode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;
		_fpc->ResetIgnoreLookInput();
		if (!_mainCharacter)
		{
			break;
		}
		if (_bQuitMenu)
		{
			_mainCharacter->QuitMenu();
		}
		//LOG(true, "Je fais quit input");
		_fpc->SetInputMode(FInputModeGameOnly());
		break;
	case EFocusMode::UI:
		mode = EMouseCaptureMode::NoCapture;
		//_fpc->SetIgnoreLookInput(true);
		_fpc->SetInputMode(FInputModeGameAndUI());
		if (_mainCharacter && _isPause)
		{
			_mainCharacter->SetInPause(true);
		}
		break;
	default:
		break;
	}
	//LOG(FOCUS_SYSTEM_USE_LOG,  "Change focus mode to: " + GET_ENUM_STRING(_inputMode));
	UGameplayStatics::SetViewportMouseCaptureMode(hud->GetWorld(), mode);
	_fpc->SetShowMouseCursor(_inputMode == EFocusMode::UI);
}