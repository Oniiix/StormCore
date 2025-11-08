// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnOctreeAsset.h"
#include "TreeCell.h"
#include "RobotHunter/Utils/DebugUtils.h"

void USpawnOctreeAsset::Spawn(ATreeCell* _parent)
{
	if (!childTreeRef)
	{
		LOG_COLOR(SPAWN_OCTREE_USE_LOG, "Failed to Create Children - [childTreeRef] is Null", RED);
		return;
	}


	const FBox& _parentBox = _parent->GetCellBox();
	const FVector& _initialExtent = _parentBox.GetExtent();
	const int& _parentSubStep = _parent->GetSubStep();

	for (int i = 0; i < 8; i++)
	{
		ATreeCell* _child = _parent->GetWorld()->SpawnActor<ATreeCell>(childTreeRef);
		if (!_child)
			continue;
		
		_child->AttachToActor(_parent, FAttachmentTransformRules::KeepRelativeTransform);
		
		FVector _extent;
		_extent.X = i % 2 == 0 ? 0 : _initialExtent.X;
		_extent.Y = i % 4 >= 2 ? _initialExtent.Y : 0;
		_extent.Z = i >= 4 ? _initialExtent.Z : 0;
		
		const int& _subStep = _parentSubStep + 1;
		const FVector& _min = _parentBox.Min + _extent;
		const FVector& _max = _min + _initialExtent;
		
		_child->SetBoxScale(_initialExtent);
		_child->SetParent(_parent);
		_child->SetupTree(_parent->GetMaster(), _parent->GetSpliMinQty(), _subStep, FBox(_min, _max), _parent->GetAllActorsComponent(), _parent->GetDebugSettings());
		
		_parent->AddChildTree(_child);
		
		_child->UpdateTree();
	}
}
