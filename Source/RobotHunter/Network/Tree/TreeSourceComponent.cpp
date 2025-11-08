#include "TreeSourceComponent.h"
#include "TreeCell.h"
#include "TreeManagerSubSystem.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/Utils/FileLogger.h"

UTreeSourceComponent::UTreeSourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTreeSourceComponent::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle _registerTimer;

	GetWorld()->GetTimerManager().SetTimer(_registerTimer, this, &UTreeSourceComponent::InitTreeSource, 0.7f);
}

void UTreeSourceComponent::InitTreeSource()
{
	if ((treeManager = GetWorld()->GetSubsystem<UTreeManagerSubSystem>()))
		treeManager->RegisterSource(this);
}

void UTreeSourceComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (treeManager)
		treeManager->UnregisterSource(this);
}

TMap<ATreeCell*, bool> UTreeSourceComponent::UpdateTreeView(ATreeCell* _mainTree)
{
	treeLeavesInView.Empty();

	TMap<ATreeCell*, bool> _resultViewChange;
	if (!_mainTree) return _resultViewChange;

	FSphere _sphere = FSphere(GetOwner()->GetActorLocation(), radius);
	TArray<ATreeCell*> _treeLeaves = _mainTree->GetAllTreeLeaves();
	const int _size = _treeLeaves.Num();

	for (int i = 0; i < _size; i++)
	{
		ATreeCell* _cell = _treeLeaves[i];

		if (FMath::SphereAABBIntersection(_sphere, _cell->GetCellBox()))
		{
			treeLeavesInView.Add(_cell);
			_resultViewChange.Add(_cell, true);
		}
		else
			_resultViewChange.Add(_cell, false);
	}

	//DRAW_SPHERE_TIME(_sphere.Center, radius, FColor::Blue, 0.5f);

	return _resultViewChange;
}

void UTreeSourceComponent::ChangeSourceSettings(const float _radius)
{
	radius = _radius;
}
