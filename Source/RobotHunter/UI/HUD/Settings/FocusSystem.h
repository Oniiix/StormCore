#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/UI/BaseSettings.h"
#include "Engine/DataAsset.h"
#include "FocusSystem.generated.h"

#define FOCUS_SYSTEM_USE_LOG false

class AMainCharacter;

UENUM()
enum class EFocusMode : uint8
{
	None, GAME, UI
};

UCLASS(HideDropdown)
class ROBOTHUNTER_API UFocusSystem final : public UBaseSettings
{
	GENERATED_BODY()

public:
	/**
	 * Set the focus mode of game
	 * @param _inputMode EFocusMode
	 * @param _mainCharacter
	 * @param _bQuitMenu
	 * @param _isPause 
	 */
	UFUNCTION(BlueprintCallable)
	void SetFocusTo(EFocusMode _inputMode, AMainCharacter* _mainCharacter = nullptr, bool _bQuitMenu = true, const bool _isPause = false) const;
};
