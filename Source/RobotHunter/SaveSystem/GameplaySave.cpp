// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplaySave.h"

#include "RobotHunter/Game/Managers/Weather/WeatherManager.h"


void UGameplaySave::SaveGameplay(const AMainCharacter* _player, const int dayToSave)
{
	if (_player)
		gameSave.day = _player->GetDayManager()->GetCurrentDay();
	else
		gameSave.day = dayToSave;
}

void UGameplaySave::SaveBonus(AMainCharacter* player)
{
	if (!player)return;
	TMap<TEnumAsByte<EBonusType>, FBonusStruct> _bonus = player->GetBonusComponent()->GetBonus();
	bonusSave.bonusStructs.Empty();
	for (const TPair<TEnumAsByte<EBonusType>, FBonusStruct>& _var : _bonus)
	{
		if (_var.Value.stack == 0)
			continue;

		bonusSave.bonusStructs.Add(_bonus[_var.Key]);
	}
	bonusSave.baseMaxLife = player->GetBonusComponent()->GetBaseMaxLife();

	if (bonusSave.baseMaxLife <= 0.0f)
		bonusSave.baseMaxLife = player->GetLifeComponent()->GetMaxLife();

	LOG(false, "SAVE DONE");
}

void UGameplaySave::SaveMeteo(AMainCharacter* _player)
{
	if (!_player)return;
	meteoSave.currentMeteoIndex = _player->GetWeatherManager()->GetWeatherIndex();
	LOG(true, "METEO SAVE DONE");
}

void UGameplaySave::SaveMembers(AMainCharacter* _player)
{
	if (!_player)return;
	if (_player->GetUsingRightArm())
	{
		memberSave.rightArmType = _player->GetUsingRightArm()->GetMemberType();
		LOG_COLOR_TIME(false, "Right Arm:" + FString::FromInt(memberSave.rightArmType), MAGENTA, 5);
	}
	if (_player->GetUsingLeftArm())
	{
		memberSave.leftArmType = _player->GetUsingLeftArm()->GetMemberType();
		LOG_COLOR_TIME(false, "Left Arm:" + FString::FromInt(memberSave.leftArmType), MAGENTA, 5);
	}
	if (_player->GetUsingThigh())
	{
		memberSave.thighType = _player->GetUsingThigh()->GetMemberType();
		LOG_COLOR_TIME(false, "Thigh:" + FString::FromInt(memberSave.thighType), MAGENTA, 5);
	}
	if (_player->GetUsingCalf())
	{
		memberSave.calfType = _player->GetUsingCalf()->GetMemberType();
		LOG_COLOR_TIME(false, "Calf:" + FString::FromInt(memberSave.calfType), MAGENTA, 5);
	}

	LOG(false, "MEMBERS SAVE DONE");
}

void UGameplaySave::SaveQuests(const TArray<int>& _asset)
{
	quests.IdQuests = _asset;
}

void UGameplaySave::SetIsTutorial(const bool isTutorial)
{
	gameSave.isTutorial = isTutorial;
}

void UGameplaySave::SaveTime()
{
	int32 Year, Month, Day, DayOfWeek;
	int32 Hour, Minute, Second, Millisecond;
	FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond);
	playerSave.date = FDateTime(Year, Month, Day, Hour, Minute, Second);
}

void UGameplaySave::SaveTuto(bool _status)
{
	tutoSave.hasStartTuto = _status;
}
