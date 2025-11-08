#include "RightArmActor.h"
#include "RobotHunter/CCC/Character/MainCharacter.h"
#include "RobotHunter/CCC/Character/Animations/CharaAnim.h"

void ARightArmActor::InitMember()
{
	Super::InitMember();
	scopeSettings.cooldownDelegate.BindLambda([&]() { scopeSettings.canShot = true; });
	unscopeSettings.cooldownDelegate.BindLambda([&]() { unscopeSettings.canShot = true; });
	mainCharacter->OnScope().AddUniqueDynamic(this, &ARightArmActor::SetIsScoped);
}

void ARightArmActor::ResetMember()
{
	Super::ResetMember();
	isScoped = false;
}

void ARightArmActor::PressBehavior()
{
	Super::PressBehavior();

	if (!(memberType == EMemberType::NAILGUN || memberType == EMemberType::FIREFLIES))
		TiltShoot();
}

void ARightArmActor::TiltShoot()
{
	if (!cameraComp)
	{
		if (mainCharacter)
			cameraComp = mainCharacter->GetCameraComponent();
	}

	if (cameraComp)
	{
		cameraComp->SetShootTiltParameters(tiltAddedPitch, tiltReturnLerpDuration);
		cameraComp->Shoot(tiltMaxPitch, tiltAddedPitch, tiltLerpDuration);
	}
}
