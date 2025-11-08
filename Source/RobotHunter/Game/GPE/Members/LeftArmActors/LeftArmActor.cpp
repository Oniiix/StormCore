#include "LeftArmActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"

void ALeftArmActor::InitMember()
{
	Super::InitMember();
	onUseLeftArm.AddUniqueDynamic(this, &ALeftArmActor::FinishUsing);
}

void ALeftArmActor::PressBehavior()
{
	Super::PressBehavior();
	mainCharacter->GetCharaAnim()->SetAnimWeight(1, false);
	mainCharacter->GetCharaAnim()->SetHasLeftArmShoot(true);
	isUsing = true;
}

void ALeftArmActor::ReleaseBehavior()
{
	Super::ReleaseBehavior();
	mainCharacter->GetCharaAnim()->SetLeftArmReleased(true);
	mainCharacter->ReactivateMembers({ this, mainCharacter->GetUsingCalf(), mainCharacter->GetUsingThigh() });
	isUsing = false;
}

void ALeftArmActor::ResetMember()
{
	Super::ResetMember();
	if (isPressed || mainCharacter->GetCharaAnim()->GetHasLeftArmRelease() || mainCharacter->GetCharaAnim()->GetHasLeftArmShoot())
	{
		mainCharacter->GetCharaAnim()->SetAnimWeight(-1, false);
		mainCharacter->GetCharaAnim()->SetIsScoping(false);
		mainCharacter->SetIsAiming(false);
		mainCharacter->ReactivateMembers({ this, mainCharacter->GetUsingCalf(), mainCharacter->GetUsingThigh() });
	}
	isUsing = false;
	mainCharacter->GetCharaAnim()->SetLeftArmReleased(false);
	mainCharacter->GetCharaAnim()->SetHasLeftArmShoot(false);
}

void ALeftArmActor::FinishUsing(bool _hasShoot)
{
	isUsing = false;
}
