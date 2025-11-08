// 


#include "HintInformation.h"

void UHintInformation::SetHintInformation(const FText& _hintTitle, const FText& _hintSubtitle) const
{
	SET_TEXT(title->GetText(), _hintTitle);
	SET_TEXT(description->GetText(), _hintSubtitle);
}
