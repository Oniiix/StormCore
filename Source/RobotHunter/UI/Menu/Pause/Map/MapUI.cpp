// 


#include "MapUI.h"

#include "HintInformation.h"
#include "Components/CanvasPanelSlot.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Game/GPE/Map/ActorWaypoint.h"


void UMapUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!HintInformation)
	{
		return;
	}
	UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(HintInformation->Slot);
	CHECK_NULL(_slot, "Slot is null");
	_slot->SetAnchors(sizeHintInformation);
	_slot->SetOffsets(0);
}

void UMapUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const AMainCharacter* _player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//CHECK_NULL(_player, "first player is null");
	if (!_player)
		return;

	//TODO replace by camera de mathieu
	//UCustomCameraComponent* _camera = _player->GetCameraComponent();

	APlayerController* _pc = GetWorld()->GetFirstPlayerController();


	float mouseX;
	float mouseY;
	_pc->GetMousePosition(mouseX, mouseY);

	int32 _x, _y;
	_pc->GetViewportSize(_x, _y);

	FVector WorldLocation;
	FVector Direction;
	if (_pc->DeprojectScreenPositionToWorld(mouseX, mouseY, WorldLocation, Direction))
	{
		FHitResult hitResult;

		if (LINE_TRACE_SIMPLE(_player->GetCameraComponent()->GetCamera()->GetComponentLocation(),
		                      WorldLocation + Direction * 3000, layersToHit, {}, hitResult))
		{
			AActorWaypoint* _waypoint = Cast<AActorWaypoint>(hitResult.GetActor());
			CHECK_NULL(_waypoint, "waypoint is null");
			const FWaypointInformation _waypointInformation = _waypoint->GetWaypointInfo();
			HintInformation->SetHintInformation(_waypointInformation.name, _waypointInformation.description);

			const FVector _actorLocation = _waypoint->GetActorLocation();
			FVector2D _worldToScreen;

			if (_pc->ProjectWorldLocationToScreen(_actorLocation, _worldToScreen))
			{
				const float _xAnchors = NORMALIZE(_worldToScreen.X, 0, _x);
				const float _yAnchors = NORMALIZE(_worldToScreen.Y, 0, _y);

				UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(HintInformation->Slot);
				CHECK_NULL(_slot, "Slot is null");
				FAnchors _anchors = AddAnchors(FAnchors(_xAnchors, _yAnchors), sizeHintInformation);
				GLog->Log(FLOATSTR(offsetX));
				AddOffsetX(_anchors, offsetX);
				AddOffsetY(_anchors, offsetY);
				_slot->SetAnchors(_anchors);
			}
			HintInformation->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			HintInformation->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

#if WITH_EDITOR

void UMapUI::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!HintInformation)
	{
		return;
	}
	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FString StrLower = PropertyName.ToString().ToLower();

	TMap<FString, TFunction<void()>> Events =
	{
		{
			"sizehintinformation", [&]
			{
				UCanvasPanelSlot* _slot = Cast<UCanvasPanelSlot>(HintInformation->Slot);
				CHECK_NULL(_slot, "Slot is null");
				_slot->SetAnchors(sizeHintInformation);
				_slot->SetOffsets(0);
			}
		}
	};

	if (Events.Contains(StrLower))
	{
		Events[StrLower]();
	}
}
#endif

FAnchors UMapUI::AddAnchors(const FAnchors& _a, const FAnchors& _b) const
{
	FAnchors Anchors = _a;
	Anchors.Minimum += _b.Minimum;
	Anchors.Maximum += _b.Maximum;
	return Anchors;
}

void UMapUI::AddOffsetX(FAnchors& _a, const float& _offset) const
{
	_a.Minimum.X += _offset;
	_a.Maximum.X += _offset;
}

void UMapUI::AddOffsetY(FAnchors& _a, const float& _offset) const
{
	_a.Minimum.Y += _offset;
	_a.Maximum.Y += _offset;
}
