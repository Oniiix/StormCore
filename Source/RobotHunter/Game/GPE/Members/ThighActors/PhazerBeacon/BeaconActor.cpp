// Objectif 3D Copyright


#include "BeaconActor.h"
#include "RobotHunter/Utils/DebugUtils.h"

ABeaconActor::ABeaconActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABeaconActor::BeginPlay()
{
	Super::BeginPlay();
	FHitResult _result;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(),
	                                                    GetActorLocation() + FVector::DownVector * 200, snapLayers,
	                                                    false, {}, EDrawDebugTrace::None, _result, false))
	{
		SetActorLocation(_result.ImpactPoint);
	}
}

TArray<FTeleportationData> ABeaconActor::GetActorAround(const float _radius)
{
	TArray<FTeleportationData> _results;

	TArray<AActor*> _actors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), _radius, detectionLayers, nullptr, {},
	                                          _actors);
	DRAW_SPHERE_TIME(GetActorLocation(), _radius, FColor::Blue, 5);

	const int _actorsCount = _actors.Num();
	for (int i = 0; i < _actorsCount; i++)
	{
		if (!_actors[i]) continue;
		if (UMovableComponent* _comp = _actors[i]->GetComponentByClass<UMovableComponent>())
		{
			if (!_comp->IsAffectBy(EMovementSource::Phazer)) continue;
			_results.Add(FTeleportationData(_actors[i], _actors[i]->GetActorLocation() - GetActorLocation(), _comp));
			_actors[i]->SetActorLocation(_actors[i]->GetActorLocation() - FVector(0, 0, 1000));
		}
	}
	return _results;
}

void ABeaconActor::TeleportAllActors(const TArray<FTeleportationData>& _teleportationData)
{
	const int _actorsCount = _teleportationData.Num();
	FVector _newLoc;

	for (int i = 0; i < _actorsCount; i++)
	{
		if (!_teleportationData[i].target) continue;
		DRAW_SPHERE_TIME(_newLoc, 20, FColor::Orange, 5);

		//_teleportationData[i].target->SetActorLocation(GetActorLocation());
		_newLoc = GetActorLocation() + _teleportationData[i].direction;

		TeleportActor(_newLoc, _teleportationData[i]);
	}
}

void ABeaconActor::TeleportActor(FVector& _newLoc, const FTeleportationData& _teleportationData)
{
	const FVector& _boxExtent = _teleportationData.target->GetRootComponent()->GetLocalBounds().BoxExtent;

	TArray<FHitResult> _results;
	if (UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(),
	                                                  _newLoc + FVector::UpVector * _teleportationData.comp->
	                                                  GetSnapOffset(),
	                                                  _newLoc + FVector::UpVector * _teleportationData.comp->
	                                                  GetSnapOffset(),
	                                                  _boxExtent * _teleportationData.target->GetActorScale(),
	                                                  _teleportationData.target->GetActorRotation(),
	                                                  snapLayers,
	                                                  true,
	                                                  {},
	                                                  EDrawDebugTrace::ForDuration,
	                                                  _results,
	                                                  true))
	{
		const int _resultIndex = _results.Num() - 1;
		if (_results[_resultIndex].GetComponent()->ComponentHasTag("complex"))
		{
			_teleportationData.target->SetActorLocation(_results[_resultIndex].Location);
			return;
		}


		const float _actorZ = (_results[_resultIndex].Component->GetLocalBounds().GetBox().Max * _results[_resultIndex].
			GetActor()->GetActorScale()).Z + _results[_resultIndex].GetActor()->GetActorLocation().Z;
		const FVector& _dirBeaconToActor = (_newLoc - GetActorLocation()).GetSafeNormal();
		const TPair<bool, bool> _isXAndNegative = {
			FMath::Abs(_dirBeaconToActor.X) > FMath::Abs(_dirBeaconToActor.Y),
			FMath::Abs(_dirBeaconToActor.X) > FMath::Abs(_dirBeaconToActor.Y)
				? _dirBeaconToActor.X < 0.
				: _dirBeaconToActor.Y < 0.
		};

		float _actorXorY;
		if (_isXAndNegative.Key)
		{
			_actorXorY = ((_isXAndNegative.Value
				               ? _results[_resultIndex].Component->GetLocalBounds().GetBox().Max
				               : _results[_resultIndex].Component->GetLocalBounds().GetBox().Min) * _results[
				_resultIndex].GetActor()->GetActorScale()).X + _results[_resultIndex].GetActor()->GetActorLocation().X;
			if ((FMath::Abs(_actorZ) - FMath::Abs(_newLoc.Z)) < (FMath::Abs(_actorXorY) - FMath::Abs(_newLoc.X)))
			{
				FHitResult _result;
				DRAW_SPHERE_TIME(FVector(_newLoc.X, _newLoc.Y, _actorZ), 20, FColor::Blue, 10);
				if (UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(),
				                                                   FVector(_newLoc.X, _newLoc.Y,
				                                                           _actorZ + _boxExtent.Z + 1) +
				                                                   FVector::UpVector * _teleportationData.comp->
				                                                   GetSnapOffset(),
				                                                   FVector(_newLoc.X, _newLoc.Y,
				                                                           _actorZ + _boxExtent.Z + 1) +
				                                                   FVector::UpVector * _teleportationData.comp->
				                                                   GetSnapOffset(),
				                                                   _boxExtent,
				                                                   _teleportationData.target->GetActorRotation(),
				                                                   snapLayers,
				                                                   true,
				                                                   {},
				                                                   EDrawDebugTrace::None,
				                                                   _result,
				                                                   true))
				{
					_newLoc = FVector(_actorXorY + _boxExtent.X, _newLoc.Y, _newLoc.Z);
					DRAW_SPHERE_TIME(FVector(_actorXorY, _newLoc.Y, _newLoc.Z), 20, FColor::Blue, 10);
				}
				else
					_newLoc = FVector(_newLoc.X, _newLoc.Y, _actorZ + _boxExtent.Z) + FVector::UpVector *
						_teleportationData.comp->GetSnapOffset();
			}
			else
			{
				DRAW_SPHERE_TIME(FVector(_actorXorY, _newLoc.Y, _newLoc.Z), 20, FColor::Blue, 10);
				_newLoc = FVector(_actorXorY + _boxExtent.X, _newLoc.Y, _newLoc.Z);
			}
		}
		else
		{
			_actorXorY = ((_isXAndNegative.Value
				               ? _results[_resultIndex].Component->GetLocalBounds().GetBox().Max
				               : _results[_resultIndex].Component->GetLocalBounds().GetBox().Min) * _results[
				_resultIndex].GetActor()->GetActorScale()).Y + _results[_resultIndex].GetActor()->GetActorLocation().Y;
			if ((FMath::Abs(_actorZ) - FMath::Abs(_newLoc.Z)) < (FMath::Abs(_actorXorY) - FMath::Abs(_newLoc.Y)))
			{
				FHitResult _result;
				DRAW_SPHERE_TIME(FVector(_newLoc.X, _newLoc.Y, _actorZ), 20, FColor::Blue, 10);
				if (UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(),
				                                                   FVector(_newLoc.X, _newLoc.Y,
				                                                           _actorZ + _boxExtent.Z + 1) +
				                                                   FVector::UpVector * _teleportationData.comp->
				                                                   GetSnapOffset(),
				                                                   FVector(_newLoc.X, _newLoc.Y,
				                                                           _actorZ + _boxExtent.Z + 1) +
				                                                   FVector::UpVector * _teleportationData.comp->
				                                                   GetSnapOffset(),
				                                                   _boxExtent,
				                                                   _teleportationData.target->GetActorRotation(),
				                                                   snapLayers,
				                                                   true,
				                                                   {},
				                                                   EDrawDebugTrace::None,
				                                                   _result,
				                                                   true))
				{
					_newLoc = FVector(_newLoc.X, _actorXorY + _boxExtent.Y, _newLoc.Z);
					DRAW_SPHERE_TIME(FVector(_newLoc.X, _actorXorY, _newLoc.Z), 20, FColor::Blue, 10);
				}
				else
					_newLoc = FVector(_newLoc.X, _newLoc.Y, _actorZ + _boxExtent.Z) + FVector::UpVector *
						_teleportationData.comp->GetSnapOffset();
			}
			else
			{
				DRAW_SPHERE_TIME(FVector(_newLoc.X, _actorXorY, _newLoc.Z), 20, FColor::Blue, 10);
				_newLoc = FVector(_newLoc.X, _actorXorY + _boxExtent.Y, _newLoc.Z);
			}
		}
	}
	_teleportationData.target->SetActorLocation(_newLoc);
}

void ABeaconActor::Tick(float _deltatime)
{
	Super::Tick(_deltatime);
	DRAW_SPHERE(GetActorLocation(), 400, FColor::Green);
}


TObjectPtr<UMeshComponent> ABeaconActor::GetActorMesh(AActor* _actor)
{
	if (UStaticMeshComponent* _mesh = _actor->GetComponentByClass<UStaticMeshComponent>())
		return _mesh;
	else if (USkeletalMeshComponent* _skeletalmesh = _actor->GetComponentByClass<USkeletalMeshComponent>())
		return _skeletalmesh;
	return nullptr;
}


//UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(),
//			FVector(_newLoc.X, _newLoc.Y, _newLoc.Z),
//			FVector(_newLoc.X, _newLoc.Y, _newLoc.Z - 1000),
//			_max * _teleportationData[i].target->GetActorScale(),
//			_teleportationData[i].target->GetActorRotation(),
//			snapLayers,
//			false,
//			{},
//			EDrawDebugTrace::None,
//			_result,
//			true);
//
//		if (UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(),
//			FVector(_newLoc.X, _newLoc.Y, _newLoc.Z + 10000),
//			_result.Location,
//			_max * _teleportationData[i].target->GetActorScale(),
//			_teleportationData[i].target->GetActorRotation(),
//			snapLayers,
//			false,
//			{},
//			EDrawDebugTrace::ForDuration,
//			_results,
//			true))
//		{
//			_teleportationData[i].target->SetActorLocation(_results[_results.Num() - 1].Location);
//		}
//		else
//			_teleportationData[i].target->SetActorLocation(_result.Location);
