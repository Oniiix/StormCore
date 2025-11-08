// Fill out your copyright notice in the Description page of Project Settings.


#include "SightSystemGPEComponent.h"
#include "RobotHunter/Utils/DebugUtils.h"


void USightSystemGPEComponent::PerceptionBehaviour()
{
	TArray<AActor*> _me = { GetOwner() };
	TArray<AActor*> _items = {};
	FHitResult _result;

	UKismetSystemLibrary::SphereOverlapActors(this, PerceptionOffLocation(), rangePerception, layersDetection, nullptr, _me, _items);


	const FVector _start = PerceptionOffLocation();


	for (size_t i = 0; i < _items.Num(); i++)
	{
		if (_items[i])
		{
			const FVector _end = _items[i]->GetActorLocation();

			FVector _direction = (_items[i]->GetActorLocation() - PerceptionOffLocation()).GetSafeNormal();
			FVector _fwd = GetOwner()->GetActorForwardVector();

			_direction.Z = 0;
			_fwd.Z = 0;


			const float _angle = GetVectorAngle(_fwd, _direction);

			if (_angle < sightAngle / 2)
			{

				bool hit = UKismetSystemLibrary::LineTraceSingleForObjects(this, _start, _end, layersObstacle, false, _me, EDrawDebugTrace::ForOneFrame, _result, true);
				if (hit)
				{
					LOG(0, "USightSystemComponent::PerceptionBehaviour: Obstacle Detected!");
					ProccessToClear();
					return;
				}



				float _distance = FVector::Dist(_items[i]->GetActorLocation(), GetOwner()->GetActorLocation());


				Detected(_items[i]);
				return;

			}

		}
	}
	ProccessToClear();

}

float USightSystemGPEComponent::GetVectorAngle(const FVector& _u, const FVector& _v) const
{
	const float _dot = FVector::DotProduct(_u, _v) / (_u.Size() * _v.Size());
	return FMath::RadiansToDegrees(FMath::Acos(_dot));
}

void USightSystemGPEComponent::ProccessToClear()
{
	if (tryToClear || !target)
		return;
	tryToClear = true;
	SET_TIMER(WORLD,timerProcessToClear, this, USightSystemGPEComponent::EndProccessToClear, rateProcessToClear, false,-1.0f);

}

void USightSystemGPEComponent::EndProccessToClear()
{
	tryToClear = false;
	CLEAR_TIMER(WORLD,timerProcessToClear);
	ClearTarget();
}

void USightSystemGPEComponent::DrawDebug()
{
	DrawCone();
}

void USightSystemGPEComponent::DrawCone()
{
	const float _degToRadSight = FMath::DegreesToRadians(sightAngle / 2);
	
		
		if (activateConeDetection)
			DrawDebugCone(GetWorld(), PerceptionOffLocation(), GetOwner()->GetActorForwardVector(), rangePerception, _degToRadSight, _degToRadSight, 32, target ? FColor::White : FColor::Red);
	

	
}
