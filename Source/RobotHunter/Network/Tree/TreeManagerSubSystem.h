// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TreeCell.h"
#include "TreeSourceComponent.h"
#include "TreeManagerSubSystem.generated.h"


#define TREE_MANAGER_USE_LOG false

UCLASS()
class ROBOTHUNTER_API UTreeManagerSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	TArray<UTreeSourceComponent*> allSourcesComponent;
	TObjectPtr<ATreeCell> mainTree = nullptr;

	float currentTime = 0;

public:
	FORCEINLINE void SetTree(ATreeCell* _tree)
	{
		mainTree = _tree;
	}
	FORCEINLINE TObjectPtr<ATreeCell> GetMainTree() { return mainTree; }
	FORCEINLINE bool RegisterSource(UTreeSourceComponent* _source)
	{
		if (allSourcesComponent.Contains(_source)) return false;
		allSourcesComponent.Add(_source);
		return true;
	}
	FORCEINLINE bool UnregisterSource(UTreeSourceComponent* _source)
	{
		if (!allSourcesComponent.Contains(_source)) return false;
		allSourcesComponent.Remove(_source);
		return true;
	}

protected:
	UTreeManagerSubSystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override { return TStatId(); }

	UFUNCTION(BlueprintCallable)void UpdateTreeVisibily();
private:


};
