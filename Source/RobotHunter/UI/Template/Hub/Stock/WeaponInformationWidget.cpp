#include "WeaponInformationWidget.h"

void UWeaponInformationWidget::InitWidget(const FText& _title, const TObjectPtr<UMaterialInterface>& _player,
                                          const FText& _description) const
{
	CHECK_NULL(textTitle, "text is null");
	CHECK_NULL(video, "video is null");
	CHECK_NULL(description, "description is null");
	CHECK_NULL(_player, "_viddeo player is null");


	SET_TEXT_STR(textTitle->GetText(), FORMAT(nameFormat, _title.ToString()));
	video->SetBrushFromMaterial(_player);
	SET_TEXT_STR(description->GetText(), FORMAT(descriptionFormat, _description.ToString()));
}
