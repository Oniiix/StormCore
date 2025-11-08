// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonCategoryPrefab.h"

#include "RobotHunter/UI/Menu/Options/OptionFrame.h"

void UButtonCategoryPrefab::Setup(const TObjectPtr<UOptionFrame>& _parent, ECategoryOption _category)
{
	parent = _parent;
	categoryEnum = _category;
}


void UButtonCategoryPrefab::RegisterEvents()
{
	Super::RegisterEvents();
	Custom_OnClick.AddUniqueDynamic(this, &UButtonCategoryPrefab::ShowCategory);
}

void UButtonCategoryPrefab::ShowCategory()
{
	parent->ShowCategory(
		static_cast<int>(categoryEnum));
}
