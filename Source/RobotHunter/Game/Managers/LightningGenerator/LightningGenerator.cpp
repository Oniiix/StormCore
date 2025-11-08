#include "LightningGenerator.h"

#include "RobotHunter/Game/CustomGameState.h"
#include "GameFramework/PlayerState.h"

#include "RobotHunter/Game/GPE/Weathers/Storm/WetStorm/WetStormWeather.h"
#include "RobotHunter/Game/GPE/Weathers/Storm/DryStorm/DryStormWeather.h"
#include "RobotHunter/Game/GPE/Lightning/Lightning.h"
#include "RobotHunter/Game/CustomGameMode.h"

#include "RobotHunter/Utils/DebugUtils.h"


#define DEBUG_LIGHTNING 0
#define MAX_Z 19000.0f

void ULightningGenerator::StartGenerateLightning()
{
	//if (isProcessGeneratingLightning)
	//	return;

	//isProcessGeneratingLightning = true;
	CHECK_NULL(lightningRef, "lightningRef is null !!")
	SET_TIMER(WORLD, timerLightningStrikeFrequency, this, ULightningGenerator::GenerateLightning, lightningStrikeFrequency, true, -1)
}
void ULightningGenerator::StopGenerateLightning()
{
	//if (!isProcessGeneratingLightning)
	//	return;

	//isProcessGeneratingLightning = false;

	CLEAR_TIMER(WORLD, timerLightningStrikeFrequency);
}

void ULightningGenerator::GenerateLightning()
{
	//if (!isProcessGeneratingLightning)
	//	return;

	TArray<TObjectPtr<APlayerState>> _pawns = GAME_STATE(ACustomGameState)->PlayerArray;

	if (_pawns.IsEmpty())
		return;

	const int _choosePlayer = FMath::RandRange(0, _pawns.Num() - 1);

	APlayerState* _playerState = _pawns[_choosePlayer];
	if (!_playerState || !_playerState->GetPawn())
		return;


	const float _X = FMath::FRandRange(_playerState->GetPawn()->GetActorLocation().X - rangeAroundPlayer, _playerState->GetPawn()->GetActorLocation().X + rangeAroundPlayer),
		        _Y = FMath::FRandRange(_playerState->GetPawn()->GetActorLocation().Y - rangeAroundPlayer, _playerState->GetPawn()->GetActorLocation().Y + rangeAroundPlayer);

	const FVector _position = FVector(_X, _Y, MAX_Z);

	GetWorld()->SpawnActor<ALightning>(lightningRef, _position, FRotator::ZeroRotator);
}

