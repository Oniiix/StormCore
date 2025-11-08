#include "PopupData.h"

void UPopupData::BindMethodOption(const FText OptionName, const FVoidDelegate& MethodToBind)
{
	const FString _OptionName = FORMAT(optionFormat, OptionName.ToString());
	if (!MethodMap.Contains(_OptionName))
	{
		MethodMap.Add(_OptionName, MethodToBind);
	}
	else
	{
		MethodMap[_OptionName] = MethodToBind;
	}
}
