// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../MoveInteractable.h"
#include <RobotHunter/Game/GPE/Weathers/Weather.h>
#include "RobotHunter/UI/UI.h"
#include "RobotHunter/UI/Template/Hub/Subtitle/Subtitle.h"
#include "RadioActor.generated.h"

#define RADIO_USE_LOG false


class URadioUI;

UCLASS()
class ROBOTHUNTER_API ARadioActor : public AMoveInteractable
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLightOnRadio, TSubclassOf<AWeather>, _currentweather, int, _day);

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (AllowPrivateAccess))
	FOnLightOnRadio onLightOnRadio;

	UPROPERTY(ReplicatedUsing= OnRep_UpdateCurrentWeather)
	TSubclassOf<AWeather> currentWeather = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Custom", meta=(AllowPrivateAccess))
	TObjectPtr<URadioUI> RadioWidget;
	//UPROPERTY(EditAnywhere, Category = "Custom|Radio")
	//TObjectPtr<UWidgetComponent> widget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Custom|Radio")
	TSubclassOf<URadioUI> radioWidgetRef = nullptr;

	bool isOn = false;

public:
	FORCEINLINE FOnLightOnRadio& OnLightOnRadio() { return onLightOnRadio; }

	ARadioActor();
	FORCEINLINE TSubclassOf<AWeather> GetCurrentWeather() { return currentWeather; }

	UFUNCTION(Server, UnReliable)
	void ServerRpc_SetClientWeather(TSubclassOf<AWeather> _weather);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual void BeginPlay() override;
	UFUNCTION()
	void SetCurrentWeather(TSubclassOf<AWeather> _weather);


	virtual void Interacted(AMainCharacter* _character) override;
	virtual void Interacted_Internal() override;

	virtual void LaunchAnimation(AMainCharacter* _character) override;

	//virtual void SetInSight(AMainCharacter* _character);
	//virtual void SetNotInSight();
	virtual void SetClosest(const bool _state) override;

private:
	//UFUNCTION()  virtual void ShowMenu() override;

	UFUNCTION()
	void OnRep_UpdateCurrentWeather();
	URadioUI* GetOrInitRadioWidget();

	UFUNCTION(BlueprintCallable, Category = "Custom|Radio") void PlaySubtitles(UDataTable* _data);
};
