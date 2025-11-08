#include "ChooseWeatherInteractable.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../../UI/HUD/GameHUD.h"
#include "RobotHunter/Utils/DebugUtils.h"

void AChooseWeatherInteractable::BeginPlay()
{
	Super::BeginPlay();
	/*onBlendEnd.AddDynamic(this, &AChooseWeatherInteractable::ShowMenu);*/
}

void AChooseWeatherInteractable::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AChooseWeatherInteractable::Interacted(AMainCharacter* _character)
{
	//Super::Interacted(_character);
	ShowMenu();
}

void AChooseWeatherInteractable::ShowMenu()
{
	AMainHUD* _hud = Cast<AMainHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	_hud->SetState(EStateUI::HubWeather);
}
