#include "ProjectileLeftArmActor.h"
#include "../../../../CCC/Character/MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Projectile/BatteryProjectile/BatteryProjectileActor.h"
#include <RobotHunter/CCC/Character/Animations/CharaAnim.h>

AProjectileLeftArmActor::AProjectileLeftArmActor()
{
#if WITH_EDITOR
	PrimaryActorTick.bStartWithTickEnabled = true;
#endif

	useCharge = true;
	useCooldown = true;

	projectileType = nullptr;
	projectile = nullptr;

	projectileSpawn = CreateDefaultSubobject<USceneComponent>("ProjectileSpawn");
	projectileSpawn->SetupAttachment(RootComponent);

	maxRange = 2000;
	hitScanLayer = TArray<TEnumAsByte<EObjectTypeQuery>>();

	projectileTarget = FVector();
}

void AProjectileLeftArmActor::FindProjectileTarget()
{
	if (!mainCharacter)
		return;

	UCustomCameraComponent* _cameraComp = mainCharacter->GetCameraComponent();

	if (_cameraComp)
	{
		UCameraComponent* _camera = _cameraComp->GetCamera();
		USpringArmComponent* _arm = _cameraComp->GetSpringArm();
		if (_camera && _arm)
		{
			const float _currentArmLength = (_camera->GetComponentLocation() - _arm->GetComponentLocation()).Length();
			const FVector _startLoc = _camera->GetComponentLocation() + _currentArmLength * _camera->GetForwardVector();
			const FVector _camForward = _camera->GetForwardVector();
			const FVector _endLoc = _startLoc + (_camForward * maxRange);

			FHitResult _hitResult = FHitResult();
			const bool _hit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), _startLoc, _endLoc,
				hitScanLayer, false, { mainCharacter }, EDrawDebugTrace::None, _hitResult, true);

			if (_hit)
				projectileTarget = _hitResult.ImpactPoint;
			else
				projectileTarget = _endLoc;
		}
	}
}

void AProjectileLeftArmActor::SpawnProjectile(const float _currentChargeTime, const FVector& _projectileTarget)
{
	if (!projectile)
		return;

	const FVector _projectileLoc = projectile->GetActorLocation();
	const FRotator _projectileLookAt = UKismetMathLibrary::FindLookAtRotation(_projectileLoc, _projectileTarget);
	const FTransform _projectileTransform = FTransform(_projectileLookAt, _projectileLoc);

	ABatteryProjectileActor* _battery = Cast<ABatteryProjectileActor>(lastProjectile);

	if (_battery)
		lastProjectile->SetLifeSpan(0.1f);

	lastProjectile = projectile;
	projectile = nullptr;
	lastProjectile->SetActorTransform(_projectileTransform);

	if (lastProjectile)
	{
		LOG_COLOR(false, "CurrentChargeTime => " + FString::SanitizeFloat(_currentChargeTime), BLUE);
		LOG_COLOR(false, "maxChargeTime => " + FString::SanitizeFloat(maxChargeTime), FColor::Cyan);

		FVector _charaVelocity = FVector(1.0f);

		UCustomCharacterMovementComponent* _movement = GetOwner()->GetComponentByClass<UCustomCharacterMovementComponent>();

		if (_movement)
			_charaVelocity = _movement->Velocity;

		FVector _forward = FVector();

		UCustomCameraComponent* _camera = GetOwner()->GetComponentByClass<UCustomCameraComponent>();

		if (_camera)
			_forward = _camera->GetSpringArm()->GetForwardVector();

		_charaVelocity.X *= _forward.X;
		_charaVelocity.Y *= _forward.Y;

		LOG_COLOR_TIME(false, "[X] " + FString::SanitizeFloat(_charaVelocity.X) + " [Y] " + FString::SanitizeFloat(_charaVelocity.Y), FColor::Red, 2.0f);
		lastProjectile->SetBonusDamage(bonusDamage);
		lastProjectile->SetBleedBonusParams(bleedBonusParams.value, bleedBonusParams.duration, bleedRate);
		lastProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		lastProjectile->InitProjectile(mainCharacter, _currentChargeTime, maxChargeTime, FVector(_charaVelocity.X, _charaVelocity.Y, 0.0f));
	}
}

void AProjectileLeftArmActor::ServerRPC_SpawnProjectile_Implementation(const float _currentChargeTime, const FVector& _projectileTarget)
{
	SpawnProjectile(_currentChargeTime, _projectileTarget);
}

void AProjectileLeftArmActor::Destroyed()
{
	Super::Destroyed();
	if (projectile)
		projectile->Destroy();
}


void AProjectileLeftArmActor::PressBehavior()
{
	Super::PressBehavior();
	if (!projectile)
		CreateProjectile();
	projectile->AttachToComponent(mainCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false), "R_Hand_Socket");
}

void AProjectileLeftArmActor::ResetMember()
{
	if (projectile && isPressed)
		projectile->AttachToComponent(memberMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false), "spearSocket");
	Super::ResetMember();
}

void AProjectileLeftArmActor::ReleaseBehavior()
{
	if (!isPressed)
		return;

	LaunchProjectile();

	Super::ReleaseBehavior();
}

void AProjectileLeftArmActor::LaunchProjectile()
{
	FindProjectileTarget();

	if (mainCharacter->HasAuthority())
		SpawnProjectile(currentChargeTime, projectileTarget);
	else
		ServerRPC_SpawnProjectile(currentChargeTime, projectileTarget);
}

void AProjectileLeftArmActor::SetupMember()
{
	Super::SetupMember();
	CreateProjectile();
}

void AProjectileLeftArmActor::EndCooldown()
{
	Super::EndCooldown();
	CreateProjectile();
}

void AProjectileLeftArmActor::CreateProjectile()
{
	projectile = GetWorld()->SpawnActor<AProjectileActor>(projectileType);
	projectile->AttachToComponent(memberMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false), "spearSocket");
}
