// 


#include "PrefabRowBonus.h"

#include "RobotHunter/UI/Menu/Pause/Game/BonusInformation.h"

void UPrefabRowBonus::SetBonus(FBonusInformation _information) const
{
	SET_TEXT_STR(countBonus->GetText(), FORMAT(FormatNumber, _information.stack));
	SET_TEXT_STR(TextBonus->GetText(), FORMAT(TitleFormat, _information.nameBonus.ToString()));


	//LOG_DEBUG("Real multiplier:" + FLOATSTR(_information.currentBonus));
	FString format = FORMAT(DescriptionFormat,
	                        FORMAT(_information.descriptionBonus.ToString(), FMath::RoundToInt(_information.currentBonus)
	                        ));

	SET_TEXT_STR(DescriptionBonus->GetText(), format);
	imageBonus->SetBrush(_information.image);
}
