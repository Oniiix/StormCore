#include "NewGameATH.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "newPlayerData/newPlayerData.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/CadenceShotgun/CadenceShotgunActor.h"
#include "RobotHunter/Game/GPE/Members/ThighActors/OffRoadRollerdrives/OffRoadRollerdrivesActor.h"
#include "RobotHunter/SaveSystem/SaveManagerSubsystem.h"
#include "RobotHunter/SaveSystem/SettingsSave.h"
#include "RobotHunter/UI/ATH/TutorialATH.h"
#include "RobotHunter/UI/ATH/Reticle/BaseReticle/BaseReticle.h"
#include "RobotHunter/UI/HUD/GameHUD.h"

void UNewGameAth::LoadReticle()
{
	TArray<UWidget*> _widgets = gameATH->GetAllChildren();
	for (const TObjectPtr<UWidget>& Widget : _widgets)
	{
		if (INSTANCEOF(Widget, UReticle))
		{
			Widget->RemoveFromParent();
		}
	}

	const TObjectPtr<AMainCharacter> _character = Cast<AMainCharacter>(
		GetWorld()->GetFirstPlayerController()->GetPawn());
	CHECK_NULL(_character, "Character is null");

	TArray<TSubclassOf<UReticle>> listOfReticle;

	LoadListOfReticle(listOfReticle, _character);

	AGameHUD* _hud = Cast<AGameHUD>(hud);
	CHECK_NULL(_hud, "HUD is null");

	for (const TSubclassOf<UReticle>& Reticle : listOfReticle)
	{
		TObjectPtr<UReticle> _crosshair = CreateWidget<UReticle>(GetWorld(), Reticle);
		CHECK_NULL(_crosshair, "Crosshair widget is null");

		UCanvasPanelSlot* _slot = gameATH->AddChildToCanvas(_crosshair);

		CHECK_NULL(_slot, "Widget slot is null");

		_slot->SetAnchors(FAnchors(0, 0, 1, 1));
		_slot->SetOffsets(0);


		_crosshair->Setup(hud);

		EReticleType _reticleType = _crosshair->GetReticleType();

		if (_reticleType != EReticleType::None)
		{
			_hud->SetCrosshair(_crosshair->GetReticleType(), _crosshair);
		}
	}

	reticle->SetMaterial(Cast<AThighActor>(_character->GetUsingThigh()));
}

void UNewGameAth::ShowHitMarker() const
{
	onHitShow.Broadcast(1.f / hitShowDuration);
}

void UNewGameAth::LoadListOfReticle(TArray<TSubclassOf<UReticle>>& Array,
                                    const TObjectPtr<AMainCharacter>& MainCharacter) const
{
	const TObjectPtr<AMembersActor> Arm = MainCharacter->GetUsingRightArm();
	CHECK_NULL(Arm, "Weapon Right is null");
	if (const TSubclassOf<UReticle> Reticle = GetReticleFromArm(Arm))
	{
		Array.Add(Reticle);
	}
	else
		LOG_ERROR(true, "Reticle is null at right arm");
}

void UNewGameAth::LoadCooldown()
{
	const TObjectPtr<AMainCharacter> _character = Cast<AMainCharacter>(
		GETFPC->GetCharacter());
	CHECK_NULL(_character, "Character is null");

	CHECK_NULL(_character->GetUsingLeftArm(), "UsingLeftArm is null !!")
	BIND_ACTION(_character->GetUsingLeftArm(), OnCooldownStart, UNewGameAth, StartCooldownLeftArm);

	CHECK_NULL(_character->GetUsingCalf(), "UsingCalf is null !!")
	BIND_ACTION(_character->GetUsingCalf(), OnCooldownStart, UNewGameAth, StartCooldownCalf);

	//CHECK_NULL(_character->GetUsingThigh(), "UsingThigh is null !!")
	//BIND_ACTION(_character->GetUsingThigh(), OnCooldownStart, UNewGameAth, StartCooldownThigh);

	//if (AOffRoadRollerdrivesActor* _roller = Cast<AOffRoadRollerdrivesActor>(
	//	Cast<AMainCharacter>(GETFPC->GetPawn())->GetUsingThigh()))
	//{
	//	BIND_ACTION(_roller, OnUseStamina, UNewGameAth, UseStamina);
	//}

	//LOG_DEBUG("Load cooldown")
}

void UNewGameAth::SetupMembers()
{
	AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter());
	CHECK_NULL(_chara, "Character is null");

	playerData->SetupPlayer(_chara);
	LoadReticle();
	CALLBACK_LAMBDA_AFTER_TIME(0.5f, LoadCooldown(););
}


TSubclassOf<UReticle> UNewGameAth::GetReticleFromArm(const TObjectPtr<AMembersActor>& _arm) const
{
	TSubclassOf<UReticle> crosshair = _arm->GetCrossHair();

	if (const USaveManagerSubsystem* _subsystem = GetGameInstance()->GetSubsystem<USaveManagerSubsystem>())
	{
		if (INSTANCEOF(_arm, ACadenceShotgunActor))
		{
			if (_subsystem->GetSaveSettings()->UseAlternativeShotgunCrosshair())
			{
				crosshair = _arm->GetCrossHairAlternative();
			}
		}
	}

	CHECK_NULL_WITH_RETURN(crosshair, "Crosshair subclass is null", return nullptr);
	return crosshair;
}

void UNewGameAth::StartCooldownLeftArm(const float _maxCooldown)
{
	//LOG_DEBUG("Use cooldown left ?");
	playerData->GetLeftArmCooldown()->Use(_maxCooldown);
}

void UNewGameAth::StartCooldownCalf(const float _maxCooldown)
{
	//LOG_DEBUG("Use cooldown calf ?");
	playerData->GetCalfCooldown()->Use(_maxCooldown);
}

void UNewGameAth::StartCooldownThigh(const float _maxCooldown)
{
	//LOG_DEBUG("Use cooldown thigh ?");
	//reticle->SetCooldown(_maxCooldown);
}

void UNewGameAth::UseStamina(const float _currentStamina, const float _maxStamina)
{
	//reticle->SetCooldownValue(_currentStamina, _maxStamina);
}

void UNewGameAth::InitWithDelay()
{
	AMainCharacter* _chara = Cast<AMainCharacter>(GETFPC->GetCharacter());
	CHECK_NULL(_chara, "Character is null");

	//TODO a voir avec système de jour tuto
	/*if (_chara->GetDayManager() && _chara->GetDayManager()->GetCurrentDay() == 0)
	{
		dayInformation->SetVisibility(ESlateVisibility::Collapsed);
		dayInformation->SetIsEnabled(false);
	}
	else
	{
		tutorialATH->SetIsEnabled(false);
		tutorialATH->RemoveFromParent();
	}*/


	SetupMembers();


	_chara->OnChangeMember().AddUniqueDynamic(this, &UNewGameAth::SetupMembers);

	Super::InitWithDelay();
}

void UNewGameAth::Refresh()
{
	Super::Refresh();
	playerData->RefreshAllInputForSkills();
}
