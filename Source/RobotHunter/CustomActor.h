// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomActor.generated.h"

UCLASS()
class ROBOTHUNTER_API ACustomActor : public AActor
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnActor, ACustomActor*, _actor);
protected:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnSpawnActor onSpawnActor;


protected:
	UPROPERTY(EditAnywhere, Category = "Custom|Debug")bool shouldTickIfViewportsOnly = false;
	UPROPERTY(EditAnywhere, Category = "Custom|Debug") bool useDebug =false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> mesh;

	bool isRuntime;

public:
	FORCEINLINE FOnSpawnActor& OnSpawnActor() { return onSpawnActor; }
	FORCEINLINE UStaticMeshComponent* GetMesh()const { return mesh;	}

	ACustomActor();
	void CallSpawnEvent(ACustomActor* _actor);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void DrawDebug();

};
