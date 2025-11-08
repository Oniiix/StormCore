// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreatReasonDataAsset.h"

int UThreatReasonDataAsset::GetTotalWeight() const
{
	const int _totalWeight = damageOnIA.weight
		+ targetLife.weight
		+ distanceTargetToAI.weight
		+ paralarvaOnPlayer.weight
		+ pennantOnPlayer.weight
		+ trapperOnPlayer.weight
		+ torpedoOnPlayer.weight
		+ titanOnPlayer.weight;
	return _totalWeight;
}

#if WITH_EDITOR
void UThreatReasonDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const bool _weightValueChange = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FThreatType, weight);
	const FString& _name = PropertyChangedEvent.GetMemberPropertyName().ToString();
	if (_weightValueChange)
	{
		const int _totalWeight = GetTotalWeight();

		if (_totalWeight > 100)
		{
			if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, damageOnIA))
			{
				damageOnIA.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, targetLife))
			{
				targetLife.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, distanceTargetToAI))
			{
				distanceTargetToAI.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, paralarvaOnPlayer))
			{
				paralarvaOnPlayer.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, pennantOnPlayer))
			{
				pennantOnPlayer.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, trapperOnPlayer))
			{
				trapperOnPlayer.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, torpedoOnPlayer))
			{
				torpedoOnPlayer.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
			else if (_name == GET_MEMBER_NAME_CHECKED(UThreatReasonDataAsset, titanOnPlayer))
			{
				titanOnPlayer.weight -= _totalWeight > 100 ? _totalWeight - 100 : 0;
			}
		}
	}

	Modify();
}
#endif
