// Fill out your copyright notice in the Description page of Project Settings.

#include "MistFlower.h"
#include "RobotHunter/AI/AI_Torpedo/AI_Torpedo.h"
//#include "RobotHunter/AI/Component/Perception/NoiseSystemComponent.h"
#include "RobotHunter/Game/GPE/Perception/Sight/SightSystemGPEComponent.h"
#include<RobotHunter/Game/GPE/Weathers/Mist/MistWeather.h>

#include "Components/AudioComponent.h"

#define MIST_FLOWER_USE_LOG 1

AMistFlower::AMistFlower()
{
	audioSystem = CreateDefaultSubobject<UAudioComponent>("Audio System");
	AddOwnedComponent(audioSystem);

	/*noiseSystem = CreateDefaultSubobject<UNoiseSystemComponent>("Noise System");
	AddOwnedComponent(noiseSystem);*/


	audioSystem->bAutoActivate = false;
	audioSystem->SetIsReplicated(true);
	sightSystem = CreateDefaultSubobject<USightSystemGPEComponent>("Sight System");
	AddOwnedComponent(sightSystem);
}



void AMistFlower::Init()
{
	sightSystem->OnDetected().AddDynamic(this, &AMistFlower::PlayerDetected);
	if (!HasAuthority())
	{
		SetActorTickEnabled(false);
		return;
	}
	if (!desactivateModifier)
		if (IsItThisWeather(EMeteoStatus::MIST))
			ActiveSpawnTorpedo();
		else
			DesactivateSpawnTorpedo();
	

}

void AMistFlower::ActiveSpawnTorpedo()
{
	activateSpawnTorpedo = true;
}
void AMistFlower::DesactivateSpawnTorpedo()
{
	activateSpawnTorpedo = false;
}


void AMistFlower::PlayerDetected(AActor* _actor)
{
	if (isActive)
		return;

	if (audioSystem)
		audioSystem->Play(0);

	//ServerRPC_MakeNoise();

	if (activateSpawnTorpedo)
	{
		
		ServerRPC_SpawnTorpedo();
	}
}

void AMistFlower::SpawnTorpedo()
{
	//only two torpedo
	//TODO Event spawn AAI_Torpedo
	AAI_Torpedo* SpawnedActor1 = GetWorld()->SpawnActor<AAI_Torpedo>(torpedo, GetActorLocation() + GetActorRightVector() * 300 , GetActorRotation());
	AAI_Torpedo* SpawnedActor2 = GetWorld()->SpawnActor<AAI_Torpedo>(torpedo, GetActorLocation() + GetActorRightVector() * -300, GetActorRotation());
}



//void AMistFlower::ServerRPC_MakeNoise_Implementation()
//{
//	if (noiseSystem)
//		noiseSystem->MakeNoise(GetActorLocation(), loudness);
//}
//bool AMistFlower::ServerRPC_MakeNoise_Validate()
//{
//	LOG_COLOR(MIST_FLOWER_USE_LOG, "AMistFlower Make noise", FLinearColor::Blue);
//	return true;
//}
void AMistFlower::ServerRPC_SpawnTorpedo_Implementation()
{
	SpawnTorpedo();
}
bool AMistFlower::ServerRPC_SpawnTorpedo_Validate()
{
	LOG_COLOR(MIST_FLOWER_USE_LOG, "MistFlower::ServerRPC_SpawnTaurpilleurs_Validate",FLinearColor::Black);
	return true;
}





