


#include "SBoxActor.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "SeismicKickActor.h"
#include "RobotHunter/Network/Component/NetworkGPEComponent.h"
#include "SeismicKickDataAsset.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/CCC/Character/Components/Status/BleedStatus/BleedStatus.h"

ASBoxActor::ASBoxActor()
{
	PrimaryActorTick.bCanEverTick = true;
	box = CreateDefaultSubobject<UBoxComponent>("Box");
	RootComponent = box;

	bonusDamage = 1.0f;
	bleedRate = 0.0f;
	bleedBonusParams = FStatusParams(EStatusType::BLEED, 0.0f, 0.0f);
}

void ASBoxActor::InitBox(const FVector2D& _finalLocation, const float _chargeRatio, USeismicKickDataAsset* _settings, ASeismicKickActor* _owner)
{
	boxSnapLayer = _settings->GetBoxSnapLayer();
	timeBetweenGap = _settings->GetTimeBetweenGap();
	currentBoxSpeed = _settings->GetMinBoxSpeed() + (_settings->GetDiffBoxSpeed() * _chargeRatio);
	ejectionAngle = _settings->GetEjectionAngle();
	ejectionForce = _settings->GetEjectionForce();
	damage = _settings->GetDamage();
	settings = _settings;
	weaponOwner = _owner;

	box->SetBoxExtent(_settings->GetBoxExtent(), true);
	SnapBox();

	finalLocation = _finalLocation;

	box->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASBoxActor::OnBoxOverlap);

	GetWorldTimerManager().SetTimer(snapTimer, this, &ASBoxActor::SnapBox, timeBetweenGap, true);

	isInit = true;
	box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASBoxActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	const FVector2D& _newLoc2D = FMath::Vector2DInterpConstantTo(FVector2D(GetActorLocation()), finalLocation, _deltaTime, currentBoxSpeed);
	SetActorLocation(FVector(_newLoc2D.X, _newLoc2D.Y, GetActorLocation().Z));
	if (_newLoc2D.Equals(finalLocation, 10.0f))
	{
		box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		box->bHiddenInGame = true;
		box->SetVisibility(false);

		SetActorTickEnabled(false);
		SetLifeSpan(5.0f);
	}
}

void ASBoxActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//TODO ASBox : Remove all replication about settings ( we d'ont neeed to set the extent of the box for the client )
	DOREPLIFETIME(ASBoxActor, settings);
}

void ASBoxActor::SnapBox()
{
	FHitResult _result;
	if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation() + FVector::UpVector * 500, GetActorLocation() + FVector::DownVector * 2000, boxSnapLayer, false, {}, EDrawDebugTrace::None, _result, true))
	{
		SetActorLocation(_result.Location + FVector(0, 0,box->Bounds.BoxExtent.Z));
		onSnapToGround.Broadcast(_result);
	}
}

void ASBoxActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UMovableComponent* _moveComp = OtherActor->GetComponentByClass<UMovableComponent>())
	{
		if (_moveComp->IsAffectBy(EMovementSource::Seismic))
		{
			FVector _dir = GetActorForwardVector();
			_dir.Z = 0;
			FVector _launchDir = UCustomMathBlueprintLibrary::GetLocalTrigoPointXZ(ejectionAngle, 1, _dir, FVector::UpVector);
			_moveComp->Launch(_launchDir * ejectionForce * 100);

			if (ULifeComponent* _lifeComp = OtherActor->GetComponentByClass<ULifeComponent>())
			{
				_lifeComp->Damage(damage * bonusDamage, weaponOwner->GetMainCharacter());
				AddBleed(OtherActor);
			}
		}
	}
}

void ASBoxActor::OnRep_Init()
{
	if (settings)
		box->SetBoxExtent(settings->GetBoxExtent(), true);
}

void ASBoxActor::AddBleed(AActor* _target)
{
	if (_target)
	{
		if (bleedBonusParams.value > 0.0f && bleedBonusParams.duration > 0.0f && bleedRate > 0.0f)
		{
			UStatusComponent* _statusComp = _target->GetComponentByClass<UStatusComponent>();

			if (_statusComp)
			{
				UBleedStatus* _bleed = Cast<UBleedStatus>(_statusComp->AddStatus(weaponOwner->GetOwner(), bleedBonusParams, true));

				if (_bleed)
					_bleed->SetBleedRate(bleedRate);
			}
		}
	}
}

