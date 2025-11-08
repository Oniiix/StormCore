// 


#include "SeparatorPrefab.h"

void USeparatorPrefab::NativePreConstruct()
{
	Super::NativeConstruct();
	SET_TEXT_STR(nameCategory->GetText(), FORMAT(format, textDisplay.ToString()));
}

void USeparatorPrefab::SetFirst() const
{
	spacerWidget->SetSize(FVector2D(0, 100));
}

void USeparatorPrefab::SetCategoryName(const FString& _name) const
{
	SET_TEXT_STR(nameCategory->GetText(), FORMAT(format, _name));
}
