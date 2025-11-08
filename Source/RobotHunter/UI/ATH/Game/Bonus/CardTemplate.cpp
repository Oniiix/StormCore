// 


#include "CardTemplate.h"

#include "UBonusATH.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

void UCardTemplate::SelectCard()
{
	CHECK_NULL(ParentClass, "Parent class is null");

	if (bIsSelected) return;
	if (UBonusAth* _ath = Cast<UBonusAth>(ParentClass))
		_ath->SetCardSelected(this);

	bIsSelected = true;
	OnCardSelected.Broadcast();
}

void UCardTemplate::RegisterEvents()
{
	Super::RegisterEvents();

	BIND_ACTION_PARAM(buttonCard, OnHovered, UCardTemplate, PlayAnimationHover);
	BIND_ACTION_PARAM(buttonCard, OnUnhovered, UCardTemplate, PlayAnimationUnHover);
	BIND_ACTION_PARAM(buttonCard, OnClicked, UCardTemplate, SelectCard);
}

void UCardTemplate::UnSelectCard()
{
	bIsSelected = false;
	OnCardUnSelected.Broadcast();
}

void UCardTemplate::SetIsNew(const bool bIsNew) const
{
	newBonusOverlay->SetVisibility(bIsNew ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCardTemplate::SetBonus(const FBonusStruct& Bonus)
{
	bonusType = Bonus.type;
	auto [nameBonus, descriptionBonusCard, color, brush] = Bonus.cardInformation;
	OnBonusLoad.Broadcast(
		FORMAT(TextBonusFormat, nameBonus.ToString()), FORMAT(DescriptionBonusFormat, descriptionBonusCard.ToString()),
		color, brush);
	/*SET_TEXT_CLAMP(titleBonus->GetText(), _bonusInformation.nameBonus, titleBonus->GetClampLength());
	SET_TEXT_CLAMP(descriptionBonus->GetText(), _bonusInformation.descriptionBonus, descriptionBonus->GetClampLength());*/
}

void UCardTemplate::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	PlayAnimationHover();
}

void UCardTemplate::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	PlayAnimationUnHover();
}

void UCardTemplate::PlayAnimationHover()
{
	if (bIsSelected) return;
	onHoverCard.Broadcast();
}

void UCardTemplate::PlayAnimationUnHover()
{
	if (bIsSelected) return;
	onUnHoverCard.Broadcast();
}

void UCardTemplate::PlayCardSelectedAnimation()
{
	OnCardSelected.Broadcast();
}
