#include "AlienDoor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/Arena/Arena.h"

AAlienDoor::AAlienDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = root;
	collisionBox = CreateDefaultSubobject<UBoxComponent>("CollsionBox");
	collisionBox->SetupAttachment(RootComponent);
	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	triggerBox->SetupAttachment(RootComponent);
	endDoorWidget = CreateDefaultSubobject<UWidgetComponent>("EndDoorWidget");
	endDoorWidget->SetupAttachment(RootComponent);
}

void AAlienDoor::BeginPlay()
{
	Super::BeginPlay();

	/// Check collisionBox availability
	if (!collisionBox)
	{
		LOG_ERROR(DOOR_USE_LOG, "CollisionBox of " + ACTOR_NAME + " is null");
		return;
	}
	defaultCollisionProfile = collisionBox->GetCollisionProfileName();

	/// Check triggerBox availability
	if (!triggerBox)
	{
		LOG_ERROR(DOOR_USE_LOG, "TriggerBox of " + ACTOR_NAME + " is null");
		return;
	}
	triggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &AAlienDoor::TriggerBoxBeginOverlap);

	isInitializationGood = true;
}

void AAlienDoor::TriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isClosed)
		return;

	AMainCharacter* _player = Cast<AMainCharacter>(OtherActor);
	if (_player && arena)
	{
		arena->CloseArena();
	}
}

void AAlienDoor::OpenDoor()
{
	if (!isInitializationGood)
		return;

	LOG_COLOR(DOOR_USE_LOG, "Open " + ACTOR_NAME, BLACK);
	onOpen.Broadcast();
	FTimerHandle _timerDisableCollision;
	GetWorld()->GetTimerManager().SetTimer(_timerDisableCollision, this, &AAlienDoor::DisableCollision, delayDisableCollision);
}

void AAlienDoor::DisableCollision()
{
	isClosed = false;
	collisionBox->SetCollisionProfileName(defaultCollisionProfile, true);
}

void AAlienDoor::ShowWidget()
{
	endDoorWidget->SetVisibility(true);
}

void AAlienDoor::HideWidget()
{
	endDoorWidget->SetVisibility(false);
}

void AAlienDoor::ClearDoor()
{
	if (!isInitializationGood)
		return;

	/// Warning : Door can be register in multiple arena
	//collisionBox->DestroyComponent();
	//triggerBox->DestroyComponent();
}

void AAlienDoor::CloseDoor()
{
	if (isClosed || !isInitializationGood)
		return;

	LOG_COLOR(DOOR_USE_LOG, "Close " + ACTOR_NAME, BLACK);
	isClosed = true;
	collisionBox->SetCollisionProfileName(FName("BlockAllExceptCamera"), true);
	onClose.Broadcast();
}

