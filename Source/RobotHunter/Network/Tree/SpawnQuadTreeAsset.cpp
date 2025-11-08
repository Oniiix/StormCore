


#include "SpawnQuadTreeAsset.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "TreeCell.h"

void USpawnQuadTreeAsset::Spawn(ATreeCell* _parent)
{
	if (!childTreeRef)
	{
		LOG_COLOR(SPAWN_QUAD_USE_LOG, "Failed to Create Children - [childTreeRef] is Null", RED);
		return;
	}

	const FBox& _parentBox = _parent->GetCellBox();
	const FVector& _initialExtent = _parentBox.GetExtent();
	const int& _parentSubStep = _parent->GetSubStep();

	for (int i = 0; i < 4; i++)
	{
		ATreeCell* _child = _parent->GetWorld()->SpawnActor<ATreeCell>(childTreeRef);
		if (!_child)
			continue;

		_child->AttachToActor(_parent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector _extent;
		_extent.X = i % 2 == 0 ? 0 : _initialExtent.X;
		_extent.Y = i % 3 >= 1 ? 0 : _initialExtent.Y;
		_extent.Z = 0;

		const int& _subStep = _parentSubStep + 1;
		const FVector& _min = _parentBox.Min + _extent;
		const FVector& _max = _min + (_initialExtent * FVector(1.0f, 1.0f, 2.0f));

		_child->SetBoxScale(_initialExtent);
		_child->SetParent(_parent);
		_child->SetupTree(_parent->GetMaster(), _parent->GetSpliMinQty(), _subStep, FBox(_min, _max), _parent->GetAllActorsComponent(), _parent->GetDebugSettings());

		_parent->AddChildTree(_child);

		_child->UpdateTree();
	}
}
