#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TreeSourceComponent.generated.h"


class ATreeCell;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UTreeSourceComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Source|Settings", meta = (UMin = 100, ClampMin = 100))
	float radius = 1000;
	//UPROPERTY(EditAnywhere, Category="Source|Settings", meta = (UMin = 1, ClampMin = 1, UMax = 360, ClampMax = 360))
	//int angle = 180;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UTreeManagerSubSystem> treeManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Source")
	TArray<ATreeCell*> treeLeavesInView;

public:
	FORCEINLINE float GetRadius() const { return radius; }
	UTreeSourceComponent();

	TMap<ATreeCell*, bool> UpdateTreeView(ATreeCell* _mainTree);
	void ChangeSourceSettings(const float _radius);

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void InitTreeSource();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
