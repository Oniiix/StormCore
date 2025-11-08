// 


#include "TutoHint.h"

#include "DataAsset/TutorialHintAsset.h"

void UTutoHint::Setup(const UTutorialHintAsset* _asset) const
{
	SET_TEXT_STR(textTutorial->GetText(), _asset->GetTextFormated());
	progressTutorial->SetPercent(0.0f);
}

void UTutoHint::SetValue(const float Progress) const
{
	CHECK_NULL(progressTutorial, "ProgressTutorial is null");
	progressTutorial->SetPercent(Progress);
}
