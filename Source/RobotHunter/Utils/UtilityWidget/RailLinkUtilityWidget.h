#pragma once

#include "CoreMinimal.h"
#if WITH_EDITOR
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetComponents.h"
#else
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#endif
#include "RailLinkUtilityWidget.generated.h"

class ARailActor;
class ARailsTargetActor;

UCLASS()
class ROBOTHUNTER_API URailLinkUtilityWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> returnLinkButton;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> departureLinkButton;
	
#if WITH_EDITOR
private:
	void LinkRailsToTarget(const TArray<ARailActor*> _rails, ARailsTargetActor* _target, const bool _isDepartureRail = true);
	UFUNCTION() void LinkReturnRailsToTarget();
	UFUNCTION() void LinkDepartureRailsToTarget();
	
	TArray<AActor*> GetSelectedActors();
	TArray<ARailActor*> GetRailsFromSelectedActors(const TArray<AActor*> _selectedActors);
	ARailsTargetActor* GetTargetFromSelectedActors(const TArray<AActor*> _selectedActors);
	
protected:
	virtual bool Initialize() override;
#endif
};
