// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreeComponent.h"
#include "SpawnTreeDataAsset.h"
#include "TreeCell.generated.h"
USTRUCT()
struct FDebugSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool useDebug = false;
	UPROPERTY(EditAnywhere, meta = (UMin = 1, ClampMin = 1, UMax = 1000, ClampMax = 1000))
	float thickness = 1;
	UPROPERTY(EditAnywhere)
	FColor debugColor = FColor::Blue;
};

#define TREE_CELL_USE_LOG false

UCLASS()
class ROBOTHUNTER_API ATreeCell : public AActor
{
	GENERATED_BODY()
#pragma region DataValues

	UPROPERTY(EditAnywhere, Category="Tree|DataValues")
	bool isMaster = false;

	UPROPERTY(EditAnywhere, Category="Tree|DataValues", meta=(UMin=1, ClampMin=1, EditCondition = "isMaster", EditConditionHides))
	int maxQty = 5;

	UPROPERTY(VisibleAnywhere, Category="Tree|DataValues")
	int subStep = 1;

	UPROPERTY(EditAnywhere, Category = "Tree|DataValues")
	TObjectPtr<USpawnTreeDataAsset> spawnAsset = nullptr;

	UPROPERTY(EditAnywhere, Category="Tree|DataValues", meta=(EditCondition="isMaster", EditConditionHides))
	FVector boxScale = FVector(100);

	UPROPERTY()
	FBox cellBox;

	UPROPERTY(VisibleAnywhere, Category = "Tree|DataValues", meta = (EditCondition = "!isMaster", EditConditionHides))
	TObjectPtr<ATreeCell> master = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Tree|DataValues", meta = (EditCondition = "!isMaster", EditConditionHides))
	TObjectPtr<ATreeCell> parent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Tree|DataValues")
	TArray<ATreeCell*> treeChilds;

	UPROPERTY(VisibleAnywhere, Category = "Tree|DataValues", meta = (EditCondition = "isMaster", EditConditionHides))
	TArray<ATreeCell*> allTreeLeaves;

	UPROPERTY(VisibleAnywhere, Category="Tree|DataValues")
	TArray<UTreeComponent*> allActorsComponents;

	UPROPERTY(VisibleAnywhere, Category="Tree|DataValues", meta = (EditCondition="isLeaves", EditConsitionHides))
	bool isLoad = false;
	UPROPERTY(VisibleAnywhere, Category="Tree|DataValues")
	bool isLeaves = false;
#pragma endregion

#pragma region Debug

	UPROPERTY(EditAnywhere, Category="Tree")
		FDebugSettings debugSettings;

#pragma endregion


public:
	FORCEINLINE void AddTreeLeaves(TObjectPtr<ATreeCell> _treeCell) { 
		allTreeLeaves.Add(_treeCell);
		_treeCell->isLeaves = true;
	}
	FORCEINLINE void AddChildTree(TObjectPtr<ATreeCell> _treeCell) { treeChilds.Add(_treeCell); }

	ATreeCell();

#pragma region Getter/Setter
	FORCEINLINE FDebugSettings GetDebugSettings() const { return debugSettings; }
	FORCEINLINE FBox GetCellBox() const { return cellBox; }
	FORCEINLINE int GetSubStep() const { return subStep; }
	FORCEINLINE bool IsMaster() const { return isMaster; }
	FORCEINLINE bool IsLoad() const { return isLoad; }
	FORCEINLINE bool IsLeaves() const { return isLeaves; }
	FORCEINLINE ATreeCell* GetMaster() {
		if (isMaster)
			return this;
		else
			return master; }
	FORCEINLINE int GetSpliMinQty() const { return maxQty; }
	FORCEINLINE TArray<UTreeComponent*> GetAllActorsComponent() { return allActorsComponents; }
	FORCEINLINE TArray<ATreeCell*> GetAllTreeLeaves() { return allTreeLeaves; }

	FORCEINLINE void SetBoxScale(const FVector& _scale) { boxScale = _scale; }
	FORCEINLINE void SetParent(ATreeCell* _parent) { parent = _parent; }
	FORCEINLINE ATreeCell* GetParent() const { return parent; }
#pragma endregion

#pragma region Action Buttons
	UFUNCTION(CallInEditor, Category="Octree Action") void UpdateTree();
	UFUNCTION(CallInEditor, Category = "Octree Action") void ClearTree();
#pragma endregion

	void SetupTree(ATreeCell* _master, const int _splitMinQty, const int _subStep, const FBox& _box, TArray<UTreeComponent*> _comps, const FDebugSettings& _debug);

	void SwitchCellState(const bool _state);
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float _deltaTime) override;
	virtual void OnConstruction(const FTransform& _transform) override;
#if WITH_EDITOR
	bool ShouldTickIfViewportsOnly() const override;
#endif

private:
	void DrawOctreeBox() const;
	TArray<UTreeComponent*> GetActorsComponentInBox();
	void SetOwnerToComponents();
};
