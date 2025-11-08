#include "SBuildEditorWindow.h"

void SBuildEditorWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(STextBlock).Text(FText::FromString("Hello from my custom Slate window!"))
	];
}
