#include "MistFlowerV2.h"
#include "Components/AudioComponent.h"
#include "RobotHunter/Game/GPE/Perception/Sight/SightSystemGPEComponent.h"
#include "RobotHunter/Game/GPE/Weathers/Mist/MistWeather.h"
#include "RobotHunter/AI/AI_Torpedo/AI_Torpedo.h"

AMistFlowerV2::AMistFlowerV2()
{
	sightSystem = CreateDefaultSubobject<USightSystemGPEComponent>("SightSystem");
	AddOwnedComponent(sightSystem);

	audioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	audioComponent->bAutoActivate = false;
	audioComponent->SetIsReplicated(true);

	torpedo = nullptr;
}

void AMistFlowerV2::PlayerDetected(AActor* _actor)
{
	if (isActive)
	{
		AMainCharacter* _mc = Cast<AMainCharacter>(_actor);

		if (_mc)
		{
			if (audioComponent)
				audioComponent->Play(0.0f);

			if (TestWeather<AMistWeather>())
				ServerRPC_SpawnTorpedo();

			StartCooldownPhase();
		}
	}
}

void AMistFlowerV2::ServerRPC_SpawnTorpedo_Implementation()
{
	if (torpedo)
	{
		GetWorld()->SpawnActor<AAI_Torpedo>(torpedo, GetActorLocation() + GetActorRightVector() * 300, GetActorRotation());
		GetWorld()->SpawnActor<AAI_Torpedo>(torpedo, GetActorLocation() + GetActorRightVector() * -300, GetActorRotation());
	}
}

void AMistFlowerV2::StartActivePhase()
{
	Super::StartActivePhase();

	if (sightSystem)
	{
		AActor* _target = sightSystem->GetTarget();

		if (_target)
			PlayerDetected(_target);
	}
}

void AMistFlowerV2::Init()
{
	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}

	if (sightSystem)
		sightSystem->OnDetected().AddDynamic(this, &AMistFlowerV2::PlayerDetected);

	StartActivePhase();
}
