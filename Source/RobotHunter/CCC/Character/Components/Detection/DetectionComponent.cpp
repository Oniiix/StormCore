#include "DetectionComponent.h"
#include "../../../Camera/Components/CustomCameraComponent.h"
#include "../../../../AI/AI_Base.h"
#include "../../../../Utils/DebugUtils.h"
#include "../../MainCharacter.h"

UDetectionComponent::UDetectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	detectionRadius = 1000.0f;
	detectionRate = 0.1f;
	exitTime = 1.5f;

	layers = TArray<TEnumAsByte<EObjectTypeQuery>>();

	detectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	detectionSphere->SetGenerateOverlapEvents(true);

	inRangeActors = TArray<AActor*>();
	inFrustumActors = TArray<AActor*>();
	detectedActors = TArray<AActor*>();

	cameraComponent = nullptr;

	/*localPlayer = nullptr;
	viewportClient = nullptr;
	viewport = nullptr;
	sceneView = nullptr;*/

	hasDetectedActor = false;
	stealthExitTimer = FTimerHandle();
	combatExitTimer = FTimerHandle();
}

void UDetectionComponent::InitComponent()
{
	/*localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (localPlayer)
	{
		viewportClient = localPlayer->ViewportClient;

		if (viewportClient)
		{
			viewport = viewportClient->Viewport;
		}
	}*/

	//AMainCharacter* _character = Cast<AMainCharacter>(GetOwner());

	//if (_character)
	//{
	//	UCustomCameraComponent* _customCamComp = _character->GetCameraComponent();

	//	if (_customCamComp)
	//		cameraComponent = _customCamComp->GetCamera();
	//}

	//FTimerHandle _timer;
	//GetWorld()->GetTimerManager().SetTimer(_timer, this, &UDetectionComponent::CheckActorsInFrustum, detectionRate, true, detectionRate);
}

//void UDetectionComponent::UpdateSceneView()
//{
//	if (!localPlayer || !viewportClient || !viewport)
//		return;
//
//	FSceneViewFamilyContext _viewFamily = FSceneViewFamilyContext(FSceneViewFamily::ConstructionValues(viewport,
//		GetWorld()->Scene, viewportClient->EngineShowFlags).SetRealtimeUpdate(true));
//	FVector _viewLocation;
//	FRotator _viewRotation;
//	sceneView = localPlayer->CalcSceneView(&_viewFamily, _viewLocation, _viewRotation, viewport);
//}

void UDetectionComponent::UpdateDetectionSphere()
{
	//detectionSphere->SetSphereRadius(detectionRadius);
}

void UDetectionComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (OtherActor && OtherActor != GetOwner())
	//{
	//	if (OtherActor->GetClass()->IsChildOf<AAI_Base>())
	//	{
	//		if (!inRangeActors.Contains(OtherActor))
	//			inRangeActors.Add(OtherActor);
	//	}
	//}
}

void UDetectionComponent::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (OtherActor && OtherActor != GetOwner())
	//{
	//	if (OtherActor->GetClass()->IsChildOf<AAI_Base>())
	//	{
	//		if (inRangeActors.Contains(OtherActor))
	//		{
	//			inRangeActors.Remove(OtherActor);

	//			if (inFrustumActors.Contains(OtherActor))
	//				inFrustumActors.Remove(OtherActor);

	//			if (detectedActors.Contains(OtherActor))
	//			{
	//				detectedActors.Remove(OtherActor);

	//				if (detectedActors.IsEmpty())
	//					GetWorld()->GetTimerManager().SetTimer(stealthExitTimer, this, &UDetectionComponent::ExitStealth, exitTime, false);
	//			}

	//			if (inRangeActors.IsEmpty())
	//				GetWorld()->GetTimerManager().SetTimer(combatExitTimer, this, &UDetectionComponent::ExitCombat, exitTime, false);
	//		}
	//	}
	//}
}

void UDetectionComponent::CheckActorsInFrustum()
{
	//UpdateSceneView();

	//if (/*!sceneView ||*/ inRangeActors.IsEmpty())
	//	return;

	///*const FConvexVolume _volume = sceneView->ViewFrustum;
	//FBox _box = FBox();*/

	//for (AActor* _actor : inRangeActors)
	//{
	//	/*_box = _actor->GetComponentsBoundingBox();
	//	const bool _detected = _volume.IntersectBox(_box.GetCenter(), _box.GetExtent());*/
	//	const bool _registeredInArray = inFrustumActors.Contains(_actor);

	//	if (_actor->WasRecentlyRendered(detectionRate))
	//	{
	//		if (_registeredInArray)
	//			return;

	//		inFrustumActors.Add(_actor);
	//	}
	//	else
	//	{
	//		if (_registeredInArray)
	//			inFrustumActors.Remove(_actor);
	//	}
	//}

	//CheckActorsVisibility();
}

void UDetectionComponent::CheckActorsVisibility()
{
	//if (inFrustumActors.IsEmpty() || !cameraComponent)
	//	return;
 //
	//const UWorld* _world = GetWorld();
	//const FVector _origin = cameraComponent->GetComponentLocation();

	//bool _isHit = false;
	//FVector _actorLoc = FVector();
	//FHitResult _hitResult = FHitResult();

	//for (AActor* _actor : inFrustumActors)
	//{
	//	if (detectedActors.Contains(_actor))
	//		continue;

	//	_actorLoc = _actor->GetActorLocation();
	//	_isHit = UKismetSystemLibrary::LineTraceSingleForObjects(_world, _origin, _actorLoc, layers, false, { GetOwner() }, EDrawDebugTrace::None, _hitResult, true);

	//	if (_isHit)
	//	{
	//		if (_hitResult.GetActor()->GetClass()->IsChildOf<AAI_Base>())
	//		{
	//			detectedActors.Add(_actor);
	//			hasDetectedActor = true;

	//			FTimerManager& _tm = _world->GetTimerManager();

	//			if (_tm.IsTimerActive(stealthExitTimer))
	//				_tm.ClearTimer(stealthExitTimer);
	//		}
	//	}
	//}
}

void UDetectionComponent::ExitStealth()
{
	//hasDetectedActor = false;
}

void UDetectionComponent::ExitCombat()
{
	//AMainCharacter* _player = Cast<AMainCharacter>(GetOwner());

	//if (_player)
	//{
	//	_player->SetInCombat(false);

	//}
}

void UDetectionComponent::BeginPlay()
{
	//Super::BeginPlay();

	//detectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UDetectionComponent::OnComponentBeginOverlap);
	//detectionSphere->OnComponentEndOverlap.AddDynamic(this, &UDetectionComponent::OnComponentEndOverlap);

	//InitComponent();
}

#if WITH_EDITOR
void UDetectionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	//const bool _canUpdateDetectionSphere = PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UDetectionComponent, detectionRadius);

	//if (_canUpdateDetectionSphere)
	//	UpdateDetectionSphere();

	//Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UDetectionComponent::SetupAttachment(USceneComponent* _root)
{
	//detectionSphere->SetupAttachment(_root);
}