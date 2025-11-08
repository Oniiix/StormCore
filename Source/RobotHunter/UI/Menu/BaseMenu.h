// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/InputSystem/InputSubsystem.h"
#include "RobotHunter/UI/UI.h"

#pragma region include all components
//Include all components for UI
#pragma endregion include all components
#include "BaseMenu.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UBaseMenu : public UUI
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuitMenu);

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "Custom" , meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UButtonPrefab>> UpDownButtonOrder;
	
	UPROPERTY(BlueprintReadOnly, Category = "Custom" , meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UButtonPrefab>> LeftRightButtonOrder;

	UPROPERTY(BlueprintReadWrite, Category = "Custom" , meta = (AllowPrivateAccess))
	int currentVerticalIndexSelection = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Custom" , meta = (AllowPrivateAccess))
	int currentHorizontalIndexSelection = 0;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, meta = (AllowPrivateAccess))
	FOnQuitMenu onQuitMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Custom", meta =(AllowPrivateAccess))
	bool isOpened = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Custom", meta =(AllowPrivateAccess))
	TObjectPtr<UButtonPrefab> currentButtonSelected;

public:
	FORCEINLINE void SetCurrentSelected(UButtonPrefab* ButtonPrefab)
	{
		currentButtonSelected = ButtonPrefab;
	}

	/*virtual void Init() override;
	virtual void InitWithDelay() override;
*/
	virtual void RegisterEvents() override;
	virtual void ExitUI() override;
	virtual void Refresh() override;

private:	/*
	void HandleHorizontalNavigation(int direction);
	void HandleVerticalNavigation(int direction);*/
protected:	
	/*UFUNCTION() virtual void OnInputReceiveEvent(EInputDirection _direction);
	UFUNCTION()
	void OnInputChangeEvent(bool bIsGamepad);*/
	UFUNCTION()
	FORCEINLINE void Callback_OnQuitMenu() { onQuitMenu.Broadcast(); }

	

	
	FORCEINLINE void SetOpened(const bool _active) { isOpened = _active; }
	UFUNCTION(BlueprintCallable, Category = "Custom") FORCEINLINE bool IsOpened() const { return isOpened; }

};
