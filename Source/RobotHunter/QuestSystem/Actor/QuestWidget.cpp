


#include "QuestWidget.h"

// Sets default values
AQuestWidget::AQuestWidget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	questWidget = CreateDefaultSubobject<UWidgetComponent>("Widget");
	questWidget->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AQuestWidget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestWidget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

