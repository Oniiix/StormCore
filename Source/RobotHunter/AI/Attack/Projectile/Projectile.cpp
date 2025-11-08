// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	projectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	RootComponent = projectileCollision;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	useBlast = stats->GetUseTrigger();
	if (useBlast)
	{
		blastComponent = NewObject<UBlastComponent>(this, UBlastComponent::StaticClass(), FName("BlastComponent"));
		blastComponent->Init(stats);
		TArray< TEnumAsByte<ECollisionChannel>> _listLayer = stats->GetLayerActivate();
		const int _num = _listLayer.Num();
		projectileCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		for (int i = 0; i < _num; i++)
			projectileCollision->SetCollisionResponseToChannel(_listLayer[i], ECollisionResponse::ECR_Block);
	}

}

void AProjectile::SetActorLocationProjectile(const FVector& _newLocation)
{
	FHitResult _result;

	SetActorLocation(_newLocation, true, &_result);
	if (_result.GetActor())
		OnOverlap(_result);
}

void AProjectile::InitProjectile(const FVector& _targetLocation)
{
	startLocation = GetActorLocation();
	targetLocation = _targetLocation;
	speed = stats->GetSpeed();
}

void AProjectile::OnOverlap(FHitResult _actorResult)
{
	//LOG(true, "pass");
	AMainCharacter* _player = Cast<AMainCharacter>(_actorResult.GetActor());
	if(_player)
		UE_LOG(LogTemp, Warning, TEXT("passPlayer"));

	UE_LOG(LogTemp, Warning, TEXT("pass"));
	const FVector& _actorLocation = GetActorLocation();
	FHitResult _result;
	const bool _hit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), _actorLocation, _actorLocation, projectileCollision->GetScaledSphereRadius(), layer, false, TArray<AActor*>(), EDrawDebugTrace::None, _result, true);
	if (_hit)
		onExplode.Broadcast(_result);

	if (useBlast)
		blastComponent->OnBlast().Broadcast(_actorLocation, _result);

	if (_player)
	{
		_player->GetLifeComponent()->Damage(stats->GetDamage(), this);
		if (!stats->GetIsDestroyOnPlayer())
			return;
	}
	if (stats->GetCanDuring())
	{
		SetLifeSpan(stats->GetTimeDuring());
		canMove = false;
		return;
	}
	Destroy();
}

void AProjectile::ClearArena(AAlienDoor* _exitGate, bool _giveBonus)
{
	Destroy();
}

void AProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectile, targetLocation);
	DOREPLIFETIME(AProjectile, startLocation);
}
