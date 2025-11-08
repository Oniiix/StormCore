#include "BumpBoxComponent.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Components/Movement/CustomCharacterMovementComponent.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"

void UBumpBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<AActor>(GetOuter());
}

void UBumpBoxComponent::Bump(AMainCharacter* _chara, const float _ratio)
{
	if (!owner) return;

	UCustomCharacterMovementComponent* _movement = _chara->GetCustomMovementComponent();
	_movement->SetVelocity(FVector(0.0f));
	_movement->PendingLaunchVelocity = FVector(0.0f);
	_movement->OnCustomLanded().AddUniqueDynamic(this, &UBumpBoxComponent::OnBumpLanded);
	_chara->StopJumping();
	const FVector& _force = ((_chara->GetActorLocation() - GetOwnerLocation()).GetSafeNormal() * bumpForwardForce + GetOwnerUpVector() * bumpUpForce) * _ratio;
	_movement->NetLaunchCharacter(_force, bumpGravity, bumpDeceleration, true);

	if (!_chara->GetCharaAnim()->GetIsBumped())
		_chara->BumpPlayer(GetOwnerLocation(), false);
}

void UBumpBoxComponent::OnBumpLanded(ACharacter* _character)
{	
	if (AMainCharacter* _chara = Cast<AMainCharacter>(_character))
		_chara->GetCustomMovementComponent()->OnCustomLanded().RemoveDynamic(this, &UBumpBoxComponent::OnBumpLanded);
}
