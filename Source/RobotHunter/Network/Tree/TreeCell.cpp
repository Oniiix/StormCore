#include "TreeCell.h"
#include "Kismet/GameplayStatics.h"
#include "TreeManagerSubSystem.h"
#include "RobotHunter/Utils/DebugUtils.h"

ATreeCell::ATreeCell()
{
#if WITH_EDITOR
	PrimaryActorTick.bCanEverTick = true;
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
}

void ATreeCell::BeginPlay()
{
	Super::BeginPlay();
	if (isMaster)
	{
		if (UTreeManagerSubSystem* _treeManager = GetWorld()->GetSubsystem<UTreeManagerSubSystem>())
			_treeManager->SetTree(this);
	}
	SwitchCellState(false);
}

void ATreeCell::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (debugSettings.useDebug)
	{
		DrawOctreeBox();
		if (isLeaves)
			DrawDebugBox(GetWorld(), GetActorLocation(), cellBox.GetExtent() * 0.80, isLoad ? FColor::Green : FColor::Red, false, -1, 0, debugSettings.thickness);
	}
}

void ATreeCell::OnConstruction(const FTransform& _transform)
{
	Super::OnConstruction(_transform);

	const FVector& _scale = boxScale / 2.0f;
	const FVector& _min = GetActorLocation() - _scale;
	const FVector& _max = GetActorLocation() + _scale;
	cellBox = FBox(_min, _max);
}

#if WITH_EDITOR
bool ATreeCell::ShouldTickIfViewportsOnly() const
{
	return debugSettings.useDebug;
}
#endif


void ATreeCell::UpdateTree()
{
	ClearTree();

	int parentActorsNumbers = allActorsComponents.Num();

	allActorsComponents = GetActorsComponentInBox();
	const bool _amountReduce = isMaster ? true : parentActorsNumbers > allActorsComponents.Num() ? true : false;
	
	if (allActorsComponents.Num() > maxQty && _amountReduce)
	{
		if (!spawnAsset)
		{
			LOG_COLOR(TREE_CELL_USE_LOG, "Failed to Create Children - There is no [SpawnAsset]", RED);
			return;
		}
		spawnAsset->Spawn(this);
	}
	else if (master && allActorsComponents.Num() >= 1)
	{
		master->AddTreeLeaves(this);
		SetOwnerToComponents();
	}
	else if (isMaster)
		AddTreeLeaves(this);

}

void ATreeCell::ClearTree()
{
	for (ATreeCell* _cell : treeChilds)
	{
		if (_cell)
		{
			_cell->ClearTree();
			_cell->Destroy();
		}
	}
	treeChilds.Empty();
	allTreeLeaves.Empty();
	if (isMaster)
	{
		allActorsComponents.Empty();
		isLeaves = false;
	}
}


void ATreeCell::SetupTree(ATreeCell* _master, const int _splitMinQty, const int _subStep, const FBox& _box, TArray<UTreeComponent*> _comps, const FDebugSettings& _debug)
{
	debugSettings = _debug;
	master = _master;
	maxQty = _splitMinQty;
	subStep = _subStep;
	SetActorLocation(_box.Min + ((_box.Max - _box.Min) / 2));
	cellBox = _box;
	allActorsComponents = _comps;
}


void ATreeCell::SwitchCellState(const bool _state)
{
	const int _compCount = allActorsComponents.Num();
	for (int i = 0; i < _compCount; i++)
	{
		UTreeComponent* _treeComp = allActorsComponents[i];
		if (!_treeComp) continue;
		_treeComp->ChangeToken(_state);
	}

	isLoad = _state;
	if (_state)
		LOG_COLOR(TREE_CELL_USE_LOG, GetName() + "Load", GREEN);
	else
		LOG_COLOR(TREE_CELL_USE_LOG, GetName() + "Unload", RED);
}


TArray<UTreeComponent*> ATreeCell::GetActorsComponentInBox()
{
	TArray<UTreeComponent*> _allActorsComponents;
	if (isMaster)
	{
		TArray<AActor*> _actors;
		TArray<UTreeComponent*> _components;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), _actors);
		for (AActor* _actor : _actors)
		{
			_actor->GetComponents(UTreeComponent::StaticClass(), _components, true);
			if (_components.Num() == 1)
				_allActorsComponents.Add(_components[0]);
		}
	}
	else
	{
		TArray<UTreeComponent*> _components = allActorsComponents;
		for (UTreeComponent* _comp : _components)
		{
			if (!_comp) continue;

			FBox _actorBox = _comp->GetOwner()->GetComponentsBoundingBox(true, true);
			if (cellBox.Intersect(_actorBox))
			{
				_allActorsComponents.Add(_comp);
			}
		}
	}
	return _allActorsComponents;
}

void ATreeCell::SetOwnerToComponents()
{
	for (TObjectPtr<UTreeComponent> _treeComp : allActorsComponents)
	{
		if (_treeComp)
			_treeComp->SetTreeOwner(this);
	}
}

void ATreeCell::DrawOctreeBox() const
{
	DrawDebugBox(GetWorld(), cellBox.GetCenter(), cellBox.GetExtent(), debugSettings.debugColor, false, -1, 0, debugSettings.thickness);
}	