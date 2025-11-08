// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TreeSourceComponent.h"
#include "TreeComponent.generated.h"


class ATreeCell;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UTreeComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadStateChanged, bool, _state);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLoadStateChanged onLoadStateChanged;

	UPROPERTY(VisibleAnywhere, Category = "Octree")
	TObjectPtr<ATreeCell> owner = nullptr;
	UPROPERTY(EditAnywhere, Category = "Octree")
	bool isCustomLoading = false;

	UPROPERTY(VisibleAnywhere, Category = "Octree")
	int token = 0;

public:
	UTreeComponent();

	FORCEINLINE FOnLoadStateChanged& OnLoadStateChanged() { return onLoadStateChanged; }
	FORCEINLINE bool IsCustomLoading() const { return isCustomLoading; }

	FORCEINLINE void SetTreeOwner(ATreeCell* _owner) { owner = _owner; }
	FORCEINLINE void SetIsCustomLoading(const bool _value) { isCustomLoading = _value; }

	void ChangeToken(const bool _state);

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UFUNCTION() 
	void BasicChangeState(const bool _state);
};
