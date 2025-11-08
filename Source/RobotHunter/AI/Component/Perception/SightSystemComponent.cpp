// Fill out your copyright notice in the Description page of Project Settings.

#include "SightSystemComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/AI/AI_Base.h"
#include "RobotHunter/CCC/CustomPlayerController.h"
#include "../Tracking/TrackingSystemComponent.h"

USightSystemComponent::USightSystemComponent()
{
}

void USightSystemComponent::InitPerception()
{
	Super::InitPerception();

	/// Check layersObstacle availability
	if (layersObstacle.IsEmpty())
	{
		LOG_WARNING(PERCEPTION_SYSTEM_COMPONENT_USE_LOG, "No layers obstacles on " + OWNER_NAME);
		return;
	}

	currentRangePerception = rangePerception;

	isInitializationGood = true;
}

void USightSystemComponent::PerceptionBehaviour()
{
	Super::PerceptionBehaviour();

	/// Sphere overlap around owner to catch AMainCharacter
	TArray<AActor*> _items = {};
	const bool _overlap = UKismetSystemLibrary::SphereOverlapActors(this, GetOwnerLocation(), rangePerception, layersDetection,
		AMainCharacter::StaticClass(), { GetOwner() }, _items);

	if (_overlap)
	{
		/// Get closest player if both on side, else the one (always 2 players max)
		AActor* _actor = _items[0];

		/// Caching var
		const FVector& _start = GetOwnerLocation();
		const FVector& _end = _actor->GetActorLocation();
		FVector _direction = (_end - _start).GetSafeNormal();
		_direction.Z = 0;
		FVector _fwd = GetOwner()->GetActorForwardVector();
		_fwd.Z = 0;
		const float _angle = GetVectorAngle(_fwd, _direction);

		FHitResult _result;
		const bool _toDraw = drawDebug && owner->GetDrawDebug() && owner->IsEnable();
		EDrawDebugTrace::Type _debugTrace = _toDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

		/// Player in sight vision
		if (_angle <= sightAngle / 2)
		{
			const bool _lineTrace = UKismetSystemLibrary::LineTraceSingleForObjects(this, _start, _end, layersObstacle, false, {GetOwner()},
				_debugTrace, _result, true);
			if (!_lineTrace)
			{
				/// Player in rangePerception -> detected
				const float _distanceToPlayer = FVector::Dist(_start, _end);
				if (_distanceToPlayer <= rangePerception)
				{
					Detected(_actor);
					return;
				}
			}
		}
	}
}

float USightSystemComponent::GetVectorAngle(const FVector& _u, const FVector& _v) const
{
	const float _dot = FVector::DotProduct(_u, _v) / (_u.Size() * _v.Size());
	return RAD_TO_DEG(FMath::Acos(_dot));
}

bool USightSystemComponent::DrawDebug()
{
	if (!Super::DrawDebug())
		return false;

	double _magicValue = 0.00875f;

	/// Draw 2D cone sight
	float _halfAngle = sightAngle / 2;
	float _angleRight = DEG_TO_RAD(_halfAngle + GetOwner()->GetActorRotation().Yaw);
	float _angleLeft = DEG_TO_RAD(-_halfAngle + GetOwner()->GetActorRotation().Yaw);
	float _x = COS(_angleRight, rangePerception);
	float _y = SIN(_angleRight, rangePerception);
	FVector _rightSight = GetOwnerLocation() + FVector(_x, _y, 0);
	DRAW_LINE_SIZE_TIME(GetOwnerLocation(), _rightSight, FColor::Red, 5.f, DELTA_TICK);
	_x = COS(_angleLeft, rangePerception);
	_y = SIN(_angleLeft, rangePerception);
	FVector _leftSight = GetOwnerLocation() + FVector(_x, _y, 0);
	DRAW_LINE_SIZE_TIME(GetOwnerLocation(), _leftSight, FColor::Red, 5.f, DELTA_TICK);
	DRAW_CIRCLE_ARC_TIME(GetOwnerLocation(), rangePerception, FColor::Red, GetOwner()->GetActorForwardVector(), sightAngle * _magicValue, DELTA_TICK);

	return true;
}
