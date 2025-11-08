// Fill out your copyright notice in the Description page of Project Settings.


#include "newPlayerData.h"

#include "GameFramework/PlayerState.h"

void UNewPlayerData::SetupPlayer(const TObjectPtr<AMainCharacter>& Player)
{
	currentPlayer = Player;

	APlayerState* _ps = currentPlayer->GetPlayerState();
	CHECK_NULL(_ps, "_ps is null!!");

	CHECK_NULL_WITH_RETURN(currentPlayer, "Current player is null", "");
	lifeComponent = currentPlayer->GetLifeComponent();

	CHECK_NULL_WITH_RETURN(lifeComponent, "Life component is null", "");
	lifebar->SetPercent(
		static_cast<float>(lifeComponent->GetCurrentLife()) / static_cast<float>(lifeComponent->GetMaxLife()));
	lifeComponent->OnUpdateLife().AddUniqueDynamic(this, &UNewPlayerData::RefreshLifeBar);
	lifeInformation->SetText(GetLifeFormated());

	PowerLeftArm->SetupMember(Player->GetUsingLeftArm());
	PowerCalfCooldown->SetupMember(Player->GetUsingCalf());
}

/*
 currentPlayer = Player;
if (_isOwner)
{
	spacerPlayer->RemoveFromParent();
}

APlayerState* _ps = Player->GetPlayerState();
CHECK_NULL(_ps, "_ps is null!!");

SET_TEXT_STR(playerName->GetText(), FORMAT(formatName, _ps->GetPlayerName()));
SET_TEXT_STR(lifeInformation->GetText(), GetLifeFormated());

CHECK_NULL_WITH_RETURN(currentPlayer, "Current player is null", "");
const ULifeComponent* _lifeComponent = currentPlayer->GetLifeComponent();

CHECK_NULL_WITH_RETURN(_lifeComponent, "Life component is null", "");
lifebar->SetPercent(
	static_cast<float>(_lifeComponent->GetCurrentLife()) / static_cast<float>(_lifeComponent->GetMaxLife()));
}*/

void UNewPlayerData::RefreshLifeBar(AActor* _owner, const int _currentLife, const int _change, AActor* _damageSource)
{
	lifebar->SetPercent(
		static_cast<float>(_currentLife) / static_cast<float>(lifeComponent->GetMaxLife()));
	lifeInformation->SetText(GetLifeFormated());
}

FString UNewPlayerData::GetLifeFormated() const
{
	CHECK_NULL_WITH_RETURN(currentPlayer, "Current player is null", "");
	const ULifeComponent* _lifeComponent = currentPlayer->GetLifeComponent();
	CHECK_NULL_WITH_RETURN(_lifeComponent, "Life component is null", "");
	const FString _format = lifeFormat.ToString();
	return FORMAT(_format, _lifeComponent->GetCurrentLife(), _lifeComponent->GetMaxLife());
}

void UNewPlayerData::RefreshAllInputForSkills() const
{
	PowerLeftArm->RefreshInput(false);
	PowerCalfCooldown->RefreshInput(false);
}
