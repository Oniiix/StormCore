


#include "SeismicKickDataAsset.h"

#if WITH_EDITOR
void USeismicKickDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USeismicKickDataAsset, minLength))
		minLength = minLength > maxLength ? maxLength : minLength;
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USeismicKickDataAsset, maxLength))
		maxLength = maxLength < minLength ? minLength : maxLength;

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USeismicKickDataAsset, minChargeDuration))
		minChargeDuration = minChargeDuration > maxChargeDuration ? maxChargeDuration : minChargeDuration;
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USeismicKickDataAsset, maxChargeDuration))
		maxChargeDuration = maxChargeDuration < minChargeDuration ? minChargeDuration : maxChargeDuration;

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USeismicKickDataAsset, maxBoxSpeed))
	{
		maxBoxSpeed = maxBoxSpeed < minBoxSpeed ? minBoxSpeed : maxBoxSpeed;
		diffBoxSpeed = maxBoxSpeed - minBoxSpeed;
	}
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USeismicKickDataAsset, minBoxSpeed))
	{
		minBoxSpeed = minBoxSpeed > maxBoxSpeed ? maxBoxSpeed : minBoxSpeed;
		diffBoxSpeed = maxBoxSpeed - minBoxSpeed;
	}
}
#endif