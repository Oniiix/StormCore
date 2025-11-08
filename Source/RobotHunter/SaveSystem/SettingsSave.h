// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RobotHunter/Utils/DebugUtils.h"

#include "SettingsSave.generated.h"

USTRUCT()
struct FAccessibility
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	int langSelection = 0;
	int default_langSelection = 0;

	UPROPERTY(SaveGame)
	int daltonismType = 0;
	int default_daltonismType = 0;

	UPROPERTY(SaveGame)
	int daltonismForce = 0;
	int default_daltonismForce = 0;

	UPROPERTY(SaveGame)
	bool fpsShow = false;
	bool default_fpsShow = false;


};

USTRUCT()
struct FGameplay
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	float fov = 90.f;
	float default_fov = 90.f;

	UPROPERTY(SaveGame)
	int sensibilityX = 150;
	int default_sensibilityX = 150;

	UPROPERTY(SaveGame)
	int sensibilityY = 150;
	int default_sensibilityY = 150;

	UPROPERTY(SaveGame)
	int controllerSensibilityX = 150;
	int default_controllerSensibilityX = 150;

	UPROPERTY(SaveGame)
	int controllerSensibilityY = 150;
	int default_controllerSensibilityY = 150;

	UPROPERTY(SaveGame)
	bool invertX = false;
	bool default_invertX = false;

	UPROPERTY(SaveGame)
	bool invertY = false;
	bool default_invertY = false;

	UPROPERTY(SaveGame)
	bool controllerInvertX = false;
	bool default_controllerInvertX = false;

	UPROPERTY(SaveGame)
	bool controllerInvertY = true;
	bool default_controllerInvertY = true;

	UPROPERTY(SaveGame)
	bool aimHolding = true;
	bool default_aimHolding = true;


};

USTRUCT()
struct FCustomDebug
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	bool useShotgunCrosshairAlternative = false;
	bool default_useShotgunCrosshairAlternative = false;
};

USTRUCT()
struct FCredit
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	bool canSkipSettings = false;
};

USTRUCT()
struct FSound
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	int master = 100;
	int default_master = 100;

	UPROPERTY(SaveGame)
	int SFX = 100;
	int default_SFX = 100;

	UPROPERTY(SaveGame)
	int music = 100;
	int default_music = 100;

	UPROPERTY(SaveGame)
	int ui = 100;
	int default_ui = 100;


};

USTRUCT()
struct FGraphics
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	int fps = 60;
	int default_fps = 60;


};
UCLASS()
class ROBOTHUNTER_API USettingsSave final : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FAccessibility Accessibility = FAccessibility();

	UPROPERTY(SaveGame)
	FGameplay Gameplay = FGameplay();

	UPROPERTY(SaveGame)
	FGraphics Graphics = FGraphics();

	UPROPERTY(SaveGame)
	FSound Sound = FSound();

	UPROPERTY(SaveGame)
	FCustomDebug debug = FCustomDebug();
	
	UPROPERTY(SaveGame)
	FCredit credits = FCredit();

public:
	FORCEINLINE FAccessibility& GetAccessibility() { return Accessibility; }
	FORCEINLINE FGameplay& GetGameplay() { return Gameplay; }
	FORCEINLINE FGraphics& GetGraphics() { return Graphics; }
	FORCEINLINE FSound& GetSound() { return Sound; }
	
	FORCEINLINE void SetFOV(const float _FOV) { Gameplay.fov = _FOV; }
	FORCEINLINE float GetFOV() const { return Gameplay.fov; }

	FORCEINLINE void SetShowFPS(const bool _show) { Accessibility.fpsShow = _show; }
	FORCEINLINE bool GetShowFPS() const { return Accessibility.fpsShow; }

	FORCEINLINE void SetLanguageSelection(const int _value) { Accessibility.langSelection = _value; }
	FORCEINLINE int GetLanguageSelection() const { return Accessibility.langSelection; }

	FORCEINLINE void SetSensibilityX(const float _value) { Gameplay.sensibilityX = _value; }
	FORCEINLINE int GetSensibilityX() const { return Gameplay.sensibilityX; }

	FORCEINLINE void SetSensibilityY(const float _value) { Gameplay.sensibilityY = _value; }
	FORCEINLINE int GetSensibilityY() const { return Gameplay.sensibilityY; }

	FORCEINLINE void SetControllerSensibilityX(const float _value) { Gameplay.controllerSensibilityX = _value; }
	FORCEINLINE int GetControllerSensibilityX() const { return Gameplay.controllerSensibilityX; }

	FORCEINLINE void SetControllerSensibilityY(const float _value) { Gameplay.controllerSensibilityY = _value; }
	FORCEINLINE int GetControllerSensibilityY() const { return Gameplay.controllerSensibilityY; }

	FORCEINLINE void SetInvertX(const bool _value) { Gameplay.invertX = _value; }
	FORCEINLINE bool GetInvertX() const { return Gameplay.invertX; }

	FORCEINLINE void SetInvertY(const bool _value) { Gameplay.invertY = _value; }
	FORCEINLINE bool GetInvertY() const { return Gameplay.invertY; }

	FORCEINLINE void SetControllerInvertX(const bool _value) { Gameplay.controllerInvertX = _value; }
	FORCEINLINE bool GetControllerInvertX() const { return Gameplay.controllerInvertX; }

	FORCEINLINE void SetControllerInvertY(const bool _value) { Gameplay.controllerInvertY = _value; }
	FORCEINLINE bool GetControllerInvertY() const { return Gameplay.controllerInvertY; }


	FORCEINLINE void SetAimHolding(const bool _value) { Gameplay.aimHolding = _value; }
	FORCEINLINE bool GetAimHolding() const { return Gameplay.aimHolding; }

	FORCEINLINE void SetUseAlternativeShotgunCrosshair(const bool _value)
	{
		debug.useShotgunCrosshairAlternative = _value;
	}

	FORCEINLINE void SetDaltonismType(const int CurrentIndex) { Accessibility.daltonismType = CurrentIndex; }
	FORCEINLINE int GetDaltonismType() const { return Accessibility.daltonismType; }

	FORCEINLINE void SetDaltonismForce(const int CurrentIndex) { Accessibility.daltonismForce = CurrentIndex; }
	FORCEINLINE int GetDaltonismForce() const { return Accessibility.daltonismForce; }

	FORCEINLINE bool UseAlternativeShotgunCrosshair() const { return debug.useShotgunCrosshairAlternative; }


	FORCEINLINE void SetMasterVolume(const int CurrentIndex) { Sound.master = CurrentIndex; }
	FORCEINLINE int GetMasterVolume() const { return Sound.master; }

	FORCEINLINE void SetSFXVolume(const int CurrentIndex) { Sound.SFX = CurrentIndex; }
	FORCEINLINE int GetSFXVolume() const { return Sound.SFX; }
	
	FORCEINLINE void SetUIVolume(const int CurrentIndex) { Sound.ui = CurrentIndex; }
	FORCEINLINE int GetUIVolume() const { return Sound.ui; }

	FORCEINLINE void SetMusicVolume(const int CurrentIndex) { Sound.music = CurrentIndex; }
	FORCEINLINE int GetMusicVolume() const { return Sound.music; }

	FORCEINLINE void SetFPS(const int CurrentIndex) { Graphics.fps = CurrentIndex; }
	 UFUNCTION(BlueprintCallable, DisplayName="Get Max FPS", Category = "Custom|Settings")
	FORCEINLINE int GetFPS() const { return Graphics.fps; }

	UFUNCTION(BlueprintCallable, Category ="Custom|Save") FORCEINLINE void UnlockSkipCredit() { credits.canSkipSettings = true; }
	UFUNCTION(BlueprintCallable, Category ="Custom|Save") FORCEINLINE bool CanSkipCredit() const { return credits.canSkipSettings; }
	UFUNCTION(BlueprintCallable, Category ="Custom|Save") FORCEINLINE void ResetSkipCredit() { credits.canSkipSettings = false; }
	/**
	 * Get all parameters values
	 * @return Array of values settings
	 */
	UFUNCTION(Blueprintable, BlueprintCallable)
	TMap<FString, FString> GetAllValues() const
	{
		return {
			{"FOV", FLOATSTR(Gameplay.fov)},
			{"Lang", INTSTR(Accessibility.langSelection)},
			{"Sensibility X", FLOATSTR(Gameplay.sensibilityX)},
			{"Sensibility Y", FLOATSTR(Gameplay.sensibilityY)},
			{"Invert X", INTSTR(Gameplay.invertX)},
			{"Invert Y", INTSTR(Gameplay.invertY)},
			{"Controller Sensibility X", FLOATSTR(Gameplay.controllerSensibilityX)},
			{"Controller Sensibility Y", FLOATSTR(Gameplay.controllerSensibilityY)},
			{"Controller Invert X", INTSTR(Gameplay.controllerInvertX)},
			{"Controller Invert Y", INTSTR(Gameplay.controllerInvertY)},
			{"Aim Holding", INTSTR(Gameplay.aimHolding)},
			{"Alternative Crosshair Shotgun", INTSTR(debug.useShotgunCrosshairAlternative)},
			{"Sound Master", FLOATSTR(Sound.master)},
			{"Sound Music", FLOATSTR(Sound.music)},
			{"Sound SFX", FLOATSTR(Sound.SFX)},
			{"FPS", FLOATSTR(Graphics.fps)},
		};
	}

	/**
	 * Insert GetAllValues in map
	 * @param _values Array in blueprint
	 */
	UFUNCTION(Blueprintable, BlueprintCallable)
	void InsertValuesOptionInMap(TMap<FString, FString>& _values) const
	{
		_values.Append(GetAllValues());
	}
};
