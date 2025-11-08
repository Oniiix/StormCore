// Objectif 3D Copyright


#include "SnapFlashActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include <RobotHunter/UI/HUD/GameHUD.h>
#include "Kismet/KismetMathLibrary.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include <RobotHunter/CCC/Character/Animations/CharaAnim.h>

void ASnapFlashActor::BeginPlay()
{
	Super::BeginPlay();
	spotLight = GetComponentByClass<USpotLightComponent>();
	if (!spotLight)
		LOG_ERROR(true, "SnapFlash:: SpotLight null");
}

void ASnapFlashActor::InitMember()
{
	Super::InitMember();
	currentDistance = distanceMin;
	cam = mainCharacter->GetCameraComponent()->GetCamera();
}

void ASnapFlashActor::ReleaseBehavior()
{
	if (!isPressed)
		return;

	const float _percentage = (currentChargeTime * 100) / maxChargeTime;
	currentDistance = distanceMin + ((distanceMax - distanceMin) * (_percentage / 100));

	Super::ReleaseBehavior();

	UCameraComponent* _cam = mainCharacter->GetCameraComponent()->GetCamera();
	USpringArmComponent* _arm = mainCharacter->GetCameraComponent()->GetSpringArm();

	const float _currentArmLength = (_cam->GetComponentLocation() - _arm->GetComponentLocation()).Length();
	const FVector& _camLoc = _cam->GetComponentLocation() + _currentArmLength * _cam->GetForwardVector();
	const FVector& _camDirection = cam->GetForwardVector();

	const float _distanceCam = FVector::DistXY(_camLoc, mainCharacter->GetActorLocation());
	FVector _targetPosition = _camLoc + _camDirection * (currentDistance + _distanceCam);
	

	FHitResult _result;
	const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _camLoc, _targetPosition, layerSnap, false, TArray<AActor*>(), EDrawDebugTrace::None, _result, true);
	if (_hit)
		_targetPosition = _result.ImpactPoint;


	onShoot.Broadcast(memberMesh->GetSocketLocation("Shoot_Loc"), (_targetPosition - memberMesh->GetSocketLocation("Shoot_Loc")).GetSafeNormal());

	if (HasAuthority())
	{
		Shoot(_camDirection, _targetPosition, _result);
		ClientRpc_CallEvent(_result.GetActor() != nullptr, _targetPosition, _camDirection, _result);
	}
	else
	{
		onExplosion.Broadcast(_result.GetActor() != nullptr, _targetPosition, _camDirection, _result);
		spotLight->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		ServerRpc_Shoot(_camDirection, _targetPosition, _result);
	}
}


void ASnapFlashActor::Shoot(const FVector& _direction, const FVector& _targetPosition, const FHitResult& _result)
{	
	onExplosion.Broadcast(_result.GetActor() != nullptr, _targetPosition, _direction, _result);
	spotLight->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	SpawnCone(_targetPosition, _direction);
}

void ASnapFlashActor::SpawnCone(const FVector& _targetPoint, const FVector& _direction)
{
	const float _length = lengthConeStun > lengthConeStagger ? lengthConeStun : lengthConeStagger;
	const float _angleStun = UKismetMathLibrary::DegreesToRadians(angleStun);

	
	TArray<AActor*> _allActors;

	const bool _hit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), _targetPoint, _length, layerCone, nullptr, TArray<AActor*>(), _allActors);
	if (!_hit)
		return;

	FHitResult _result;

	const int _num = _allActors.Num();

	for (AActor* _actor : _allActors)
	{
		const bool _isObstacle = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _targetPoint, _actor->GetActorLocation(), layerObstacle, false, TArray<AActor*>(), EDrawDebugTrace::None, _result, true);
		
		if (_isObstacle)
			continue;

		AAI_Base* _ai = Cast<AAI_Base>(_actor);

		if (!_ai)
			continue;

		const float _angle = UCustomMathBlueprintLibrary::Angle(_targetPoint, (_targetPoint + _direction).GetSafeNormal());

		if (_angle < angleStragger)
		{
			//StaggerAI(_ai, _targetPoint);
			//return;
		}
		UE_LOG(LogTemp, Warning, TEXT("%f,%f"), _angle, angleStun);
		if (_angle < angleStun)
			StunAI(_ai, _targetPoint);
	}
	//spotLight->AttachToComponent(mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASnapFlashActor::StunAI(AAI_Base* _ai, const FVector& _pointImpact)
{
	if (FVector::Distance(_ai->GetActorLocation(), _pointImpact) < lengthConeStun)
	{
		if (_ai)
		{
			statusComp = _ai->GetComponentByClass<UStatusComponent>();

			if (statusComp)
				statusComp->AddStatus(this, stunParams);
		}
	}
}

void ASnapFlashActor::StaggerAI(AAI_Base* _ai, const FVector& _pointImpact)
{
	if (!(FVector::Distance(_ai->GetActorLocation(), _pointImpact) < lengthConeStagger))
		return;

	if (_ai)
	{
		if (!statusComp)
			statusComp = _ai->GetComponentByClass<UStatusComponent>();

		if (statusComp)
			statusComp->AddStatus(this, staggerParams);
	}
}

float ASnapFlashActor::AngleTest(const FVector& _u, const FVector& _v)
{
	const float _dot = FVector::DotProduct(_u, _v) / (_u.Size(), _v.Size());
	return FMath::RadiansToDegrees(FMath::Acos(_dot));
}

void ASnapFlashActor::ServerRpc_Shoot_Implementation(const FVector& _direction, const FVector& _targetPosition, const FHitResult& _result)
{
	Shoot(_direction, _targetPosition, _result);
}


bool ASnapFlashActor::ClientRpc_CallEvent_Validate(const bool _hit, const FVector& _position, const FVector& _direction, const FHitResult& _result)
{
	return !(HasAuthority());
}

void ASnapFlashActor::ClientRpc_CallEvent_Implementation(const bool _hit, const FVector& _position, const FVector& _direction, const FHitResult& _result)
{
	onExplosion.Broadcast(_hit, _position, _direction, _result);
	spotLight->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}