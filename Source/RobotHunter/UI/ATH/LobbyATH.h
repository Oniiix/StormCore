// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialATH.h"
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Hub/Subtitle/Subtitle.h"
#include "LobbyATH.generated.h"

class UBaseReticle;
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API ULobbyATH final : public UUI
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBaseReticle> baseReticle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTutorialATH> tutoATH;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USubtitle> Subtitle;

public:
	FORCEINLINE TObjectPtr<UTutorialATH> GetTutorialATH() const { return tutoATH; }

	virtual void Init() override;

	void ShowSubtitle();
	void HideSubtitle();
	FORCEINLINE TObjectPtr<USubtitle> GetSubtitles() const { return Subtitle; }
};
