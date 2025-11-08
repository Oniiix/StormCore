#include "NetProjectileActor.h"
#include "../../../../AI/AI_Base.h"
#include "../../../../CCC/Character/MainCharacter.h"
#include "RobotHunter/Interface/MembersInterface/AffectByNetLauncherInterface.h"

ANetProjectileActor::ANetProjectileActor()
{
	stunDuration = { { EAIType::Paralarva, 10.0f }, { EAIType::Trapper, 8.0f }, { EAIType::Pennant, 6.0f },
			  { EAIType::Torpedo, 4.0f }, { EAIType::Titan, 2.0f } };
}

void ANetProjectileActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	canMove = false;

	if (ULifeComponent* _life = OtherActor->GetComponentByClass<ULifeComponent>())
	{	
		//_life->HitLocation(GetActorLocation());
		_life->Damage(damage, mainCharacter);
	}

	if (OtherActor->Implements<UAffectByNetLauncherInterface>())
		Cast<IAffectByNetLauncherInterface>(OtherActor)->AffectByNetLauncher();

	SetLifeSpan(0.1f);
}
