#include "BatteryProjectileActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../../../CCC/Character/MainCharacter.h"
#include "../../../../Utils/DebugUtils.h"
#include "RobotHunter/AI/Component/Life/WeakPointComponent.h"

ABatteryProjectileActor::ABatteryProjectileActor()
{
	lifeTime = 3.0f;
	cooldown = 1.0f;
	radiusRange = 5.0f;

	isActive = false;
	activateLayer = TArray<TEnumAsByte<EObjectTypeQuery>>();

	triggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	RootComponent = triggerBox;

	spearMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpearMesh");
	spearMesh->SetupAttachment(triggerBox);
}

void ABatteryProjectileActor::InitProjectile(AMainCharacter* _mainCharacter, const float _chargeTime, const float _maxChargeTime, const FVector _charaVelocity)
{
	Super::InitProjectile(_mainCharacter, _chargeTime, _maxChargeTime, _charaVelocity);

	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerBox->SetGenerateOverlapEvents(true);
	onCanActivate.AddUniqueDynamic(this, &ABatteryProjectileActor::Activate);
	spearMesh->SetRelativeScale3D(FVector(2.5f));
	canMove = true;
	onLaunch.Broadcast();
}

void ABatteryProjectileActor::Activate()
{
	const FVector _start = GetActorLocation();
	const FVector _end = _start;
	FTimerHandle _activateTimer;

	TArray<FHitResult> _result = TArray<FHitResult>();
	const bool _hit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), _start, _end, radiusRange, activateLayer, false, {}, EDrawDebugTrace::None, _result, true);
	onSpawnAOE.Broadcast();
	if (_hit)
		CheckForObjects(_result);

	GetWorld()->GetTimerManager().SetTimer(_activateTimer, this, &ABatteryProjectileActor::Activate, cooldown);
}

void ABatteryProjectileActor::CheckForObjects(const TArray<FHitResult>& _result)
{
	TArray<AActor*> _damagedActors;
	const int _count = _result.Num();

	for (int i = 0; i < _count; i++)
	{
		if (!_result[i].GetActor())
			continue;

		ULifeComponent* _life = _result[i].GetActor()->GetComponentByClass<ULifeComponent>();

		if (_life && !_damagedActors.Contains(_result[i].GetActor()))
		{
			CallOnHit(_result[i]);
			LOG_COLOR(BATTERY_PROJECTILE_USE_LOG, "IA", FColor::Red);
			_life->Damage((parentActor == _result[i].GetActor() ? damage * weakMultiplier : damage) * bonusDamage, mainCharacter);
			//AddBleed(_result[i].GetActor());
			_damagedActors.Add(_result[i].GetActor());
		}
	}
}

void ABatteryProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	canMove = false;
}

void ABatteryProjectileActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ABatteryProjectileActor::StopIfHit()
{
	canMove = false;
	AActor* _otherActor = moveResult.GetActor();

	if (_otherActor && !isActive)
	{
		triggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		triggerBox->OnComponentBeginOverlap.Clear();

		UPrimitiveComponent* _otherComp = moveResult.GetComponent();

		if (_otherComp->IsA<UWeakPointComponent>())
		{
			parentActor = _otherActor;
			weakMultiplier = Cast<UWeakPointComponent>(_otherComp)->GetDamageMultiplier();
		}

		isActive = true;
		AttachToActor(_otherActor, FAttachmentTransformRules::KeepWorldTransform);
		onCanActivate.Broadcast();
		SetLifeSpan(lifeTime);
	}
}

//void ABatteryProjectileActor::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	//ACustomActor::NotifyActorBeginOverlap(OtherActor);
//	//canMove = false;
//	//
//	//if (OtherActor && !isActive)
//	//{
//	//	if ()
//	//
//	//	isActive = true;
//	//	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
//	//	onCanActivate.Broadcast();
//	//	SetLifeSpan(lifeTime);
//	//}
//}

//void ABatteryProjectileActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	canMove = false;
//	LOG_COLOR(true, "Init Battery", BLUE);
//	if (OtherActor && !isActive)
//	{
//		triggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//		triggerBox->OnComponentBeginOverlap.Clear();
//		if (OtherComp->IsA<UWeakPointComponent>())
//		{
//			parentActor = OtherActor;
//			weakMultiplier = Cast<UWeakPointComponent>(OtherComp)->GetDamageMultiplier();
//		}
//			isActive = true;
//		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
//		onCanActivate.Broadcast();
//		SetLifeSpan(lifeTime);
//	}
//}

void ABatteryProjectileActor::CallOnHit(const FHitResult& _result)
{
	onHit.Broadcast(_result);
	//if (HasAuthority())
	//	ClientRpc_CallOnHitEvent(_result);
	//else
	//	ServerRpc_CallOnHitEvent(_result);
}

void ABatteryProjectileActor::ServerRpc_CallOnHitEvent_Implementation(const FHitResult& _result)
{
	onHit.Broadcast(_result);
}

bool ABatteryProjectileActor::ClientRpc_CallOnHitEvent_Validate(const FHitResult& _result)
{
	return !(HasAuthority());
}

void ABatteryProjectileActor::ClientRpc_CallOnHitEvent_Implementation(const FHitResult& _result)
{
	onHit.Broadcast(_result);
}
