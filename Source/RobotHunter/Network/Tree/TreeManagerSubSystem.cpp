// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeManagerSubSystem.h"
#include "RobotHunter/Utils/DebugUtils.h"


UTreeManagerSubSystem::UTreeManagerSubSystem()
{
}

void UTreeManagerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UTreeManagerSubSystem::Deinitialize()
{
	Super::Deinitialize();

}

void UTreeManagerSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
	if (currentTime >= 0.5f)
	{
		UpdateTreeVisibily();
		currentTime = 0;
	}
}

void UTreeManagerSubSystem::UpdateTreeVisibily()
{
	TMap<ATreeCell*, bool> _treeResult;
	
	const int _sourceCount = allSourcesComponent.Num();
	for (int i = 0; i < _sourceCount; i++)
	{
		TMap<ATreeCell*, bool> _sourceResult = allSourcesComponent[i]->UpdateTreeView(mainTree);
		for (TPair<ATreeCell*, bool> _pair : _sourceResult)
		{
			if (_treeResult.Contains(_pair.Key))
			{
				if (_treeResult[_pair.Key])
					continue;
				else if (_pair.Value)
				{
					_treeResult[_pair.Key] = _pair.Value;
					continue;
				}
			}
			else
				_treeResult.Add(_pair);
		}
	}

	for (TPair<ATreeCell*, bool> _pair : _treeResult)
	{

		//LOG_COLOR(TREE_MANAGER_USE_LOG, "Tree cell change, new value : visibility => {" + FString::FromInt(_pair.Value.loadVisibility) + "} / collision {" + FString::FromInt(_pair.Value.loadCollision) + "}", YELLOW);
		if ((_pair.Value == _pair.Key->IsLoad()))
			continue;
		LOG_COLOR(TREE_MANAGER_USE_LOG, "Tree cell change, new value : visibility => {" + FString::FromInt(_pair.Value), YELLOW);
		_pair.Key->SwitchCellState(_pair.Value);

	}
}
