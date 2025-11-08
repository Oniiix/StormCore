#pragma once

#include "CoreMinimal.h"
#include "OptionFrame.h"
#include "RobotHunter/UI/Menu/BaseMenu.h"
#include "OptionMenu.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UOptionMenu final : public UBaseMenu
{
	GENERATED_BODY()

#pragma region properties

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UOptionFrame> OptionFrame;

#pragma endregion properties

#pragma region methods
#pragma region public

public:
	virtual void RegisterEvents() override;
	virtual void Refresh() override;
	FORCEINLINE TObjectPtr<UOptionFrame> GetOptionFrame() const { return OptionFrame; }
#pragma endregion public
#pragma endregion methods
};
