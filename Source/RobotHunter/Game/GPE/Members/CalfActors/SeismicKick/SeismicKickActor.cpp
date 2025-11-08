


#include "SeismicKickActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/Utils/CustomMathBlueprintLibrary.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"


void ASeismicKickActor::InitMember()
{
	Super::InitMember();
	InitPropertyWithDataAsset();
	cam = mainCharacter->GetCameraComponent()->GetCamera();
	SetActorTickEnabled(false);
	timerManager = &GetWorld()->GetTimerManager();
	mainCharacter->OnUseCalf().AddUniqueDynamic(this, &ASeismicKickActor::FinishSK);
}

void ASeismicKickActor::PressBehavior()
{
	Super::PressBehavior();

	GetMainCharacter()->GetCharaAnim()->SetHasStartSKKick(true);
	mainCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	mainCharacter->SetCanMove(false);
	mainCharacter->SetCanTurn(false);
	mainCharacter->GetInteractionComponent()->SetCanInteract(false);
	
	const FRotator& _camRotation = mainCharacter->GetCameraComponent()->GetCamera()->GetComponentRotation();
	direction = UCustomMathBlueprintLibrary::GetForwardFromRotation(FRotator(0, _camRotation.Yaw, _camRotation.Roll)), 1.0f;
	const FRotator& _rotation = FRotator(0, UCustomMathBlueprintLibrary::FindAngle(FVector2D(direction)), 0);
	mainCharacter->SetMeshRotation(_rotation, false, false);
	SetActorTickEnabled(true);

	timerManager->SetTimer(chargeTimer, FTimerDelegate::CreateUObject(this, &ASeismicKickActor::ChargeFinish, maxChargeDuration), maxChargeDuration, FTimerManagerTimerParameters());
}

void ASeismicKickActor::ReleaseBehavior()
{
	if (!settings || !timerManager->IsTimerActive(chargeTimer)) return;

	if (timerManager->GetTimerElapsed(chargeTimer) >= minChargeDuration)
	{
		SpawnSeismBox(direction, ((timerManager->GetTimerElapsed(chargeTimer) - minChargeDuration) / (maxChargeDuration - minChargeDuration)));
		timerManager->ClearTimer(chargeTimer);
		Super::ReleaseBehavior();
	}
	else
		timerManager->SetTimer(chargeTimer, FTimerDelegate::CreateUObject(this, &ASeismicKickActor::ChargeFinish, minChargeDuration), minChargeDuration - timerManager->GetTimerElapsed(chargeTimer), FTimerManagerTimerParameters());
}

bool ASeismicKickActor::CanUse()
{
	return Super::CanUse() && settings && !mainCharacter->GetCustomMovementComponent()->IsFalling() && !GetWorld()->GetTimerManager().IsTimerActive(chargeTimer);
}

void ASeismicKickActor::ResetMember()
{
	timerManager->ClearTimer(chargeTimer);
	GetMainCharacter()->GetCharaAnim()->SetHasStartSKKick(false);
	GetMainCharacter()->GetCharaAnim()->SetHasSpawnBox(false);
	if (isUsing)
	{
		mainCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		mainCharacter->ReactivateMembers({ this });
		SetActorTickEnabled(false);
		mainCharacter->SetCanMove(true);
		mainCharacter->SetCanTurn(true);
		mainCharacter->GetInteractionComponent()->SetCanInteract(true);
	}
	Super::ResetMember();
}

void ASeismicKickActor::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
	if (!timerManager->IsTimerActive(chargeTimer)) return;
	RotateMesh();
}

void ASeismicKickActor::SpawnSeismBox(const FVector& _direction, const float _ratio)
{
	SetActorTickEnabled(false);
	if (!HasAuthority())
	{
		StartCooldown();
		ServerRpc_SpawnSeismBox(_direction, _ratio);
		return;
	}

	mainCharacter->GetCharaAnim()->SetHasStartSKKick(false);
	mainCharacter->GetCharaAnim()->SetHasSpawnBox(true);

	const FVector _position = mainCharacter->GetActorLocation() + _direction * offset;
	const FRotator& _rotation = FRotator(0, UCustomMathBlueprintLibrary::FindAngle(FVector2D(_direction)), 0);
	ASBoxActor* _box = GetWorld()->SpawnActor<ASBoxActor>(boxRef, _position, _rotation);

	mainCharacter->SetMeshRotation(_rotation, false);
	const float _diff = maxLength - minLength;
	_box->SetBonusDamage(bonusDamage);
	_box->SetBleedBonusParams(bleedBonusParams.value, bleedBonusParams.duration, bleedRate);
	_box->InitBox(FVector2D(mainCharacter->GetActorLocation() + _direction * (minLength + (_diff * _ratio))), _ratio, settings ,this);
	StartCooldown();
}

void ASeismicKickActor::InitPropertyWithDataAsset()
{
	CHECK_NULL(settings, "Settings is null!!")

	maxChargeDuration = settings->GetMaxChargeDuration();
	minChargeDuration = settings->GetMinChargeDuration();
	minLength = settings->GetMinLength();
	offset = settings->GetOffset();
	maxLength = settings->GetMaxLength();
	boxExtent = settings->GetBoxExtent();
	boxRef = settings->GetBoxRef();
}

void ASeismicKickActor::ChargeFinish(const float _maxTimer)
{
	SpawnSeismBox(direction, (_maxTimer - minChargeDuration) / (maxChargeDuration - minChargeDuration));
}

void ASeismicKickActor::RotateMesh()
{
	const FRotator& _camRotation = cam->GetComponentRotation();
	direction = UCustomMathBlueprintLibrary::GetForwardFromRotation(FRotator(0, _camRotation.Yaw, _camRotation.Roll)), 1.0f;
	const FRotator& _rotation = FRotator(0, UCustomMathBlueprintLibrary::FindAngle(FVector2D(direction)), 0);
	mainCharacter->SetMeshRotation(_rotation, false);
}

void ASeismicKickActor::FinishSK()
{
	isUsing = false;
	mainCharacter->SetCanMove(true);
	mainCharacter->SetCanTurn(true);
	mainCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	mainCharacter->GetInteractionComponent()->SetCanInteract(true);
	mainCharacter->ReactivateMembers({ this });
}


void ASeismicKickActor::ServerRpc_SpawnSeismBox_Implementation(const FVector& _direction, const float _ratio)
{
	SpawnSeismBox(_direction, _ratio);
}