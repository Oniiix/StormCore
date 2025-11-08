// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotHunter/AI/AIsRefDataAsset.h"

TSubclassOf<AAI_Base> UAIsRefDataAsset::GetAIRefByType(EAIType _type)
{
	switch (_type)
	{
	case Paralarva:
		return paralarvaRef;
		break;
	//case Pennant:
	//	return pennantRef;
	//	break;
	case Trapper:
		return trapperRef;
		break;
	//case Torpedo:
	//	return torpedoRef;
	//	break;
	case Titan:
		return titanRef;
		break;
	case Base:
		LOG_ERROR(true, "Type 'Base' given ! Use other type");
		return nullptr;
		break;
	}
	return nullptr;
}
