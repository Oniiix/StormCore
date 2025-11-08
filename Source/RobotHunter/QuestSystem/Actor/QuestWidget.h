

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/WidgetComponent.h>
#include "QuestWidget.generated.h"

UCLASS()
class ROBOTHUNTER_API AQuestWidget : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> questWidget;
	
public:	
	// Sets default values for this actor's properties
	AQuestWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
