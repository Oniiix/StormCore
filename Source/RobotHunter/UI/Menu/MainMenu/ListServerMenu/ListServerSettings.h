#pragma once

#include "CoreMinimal.h"
#include "../../../BaseSettings.h"
#include "RobotHunter/UI/Template/MainMenu/Server/SlotServerTemplate.h"
#include "RobotHunter/Utils/DebugUtils.h"
#include "ListServerSettings.generated.h"
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UListServerSettings final : public UBaseSettings
{
	GENERATED_BODY()

	/*The template UI which will serve as a server slot */
	UPROPERTY(EditAnywhere, Category = "Custom|UI|Settings")
	TSubclassOf<USlotServerTemplate> slotServerTemplate;

	/** This text will be show the number after servers founds. */
	UPROPERTY(EditAnywhere, Category = "Custom|UI|Settings")
	FText textServerFound = TEXTSTR("Serveurs trouvé(s)");

	/** This text will be show the number after servers founds. */
	UPROPERTY(EditAnywhere, Category = "Custom|UI|Settings")
	FText textServerSearch = TEXTSTR("Recherche de serveur...");

public:
	/**
	 * return the subclassof of blueprint of slot server template
	 * @return TSubclassOf<USlotServerTemplate>
	 */
	FORCEINLINE TSubclassOf<USlotServerTemplate> GetSlotServerTemplate() const { return slotServerTemplate; }

	/**
	 * Return the text of server found
	 * @return FText
	 */
	FORCEINLINE FText GetTextServerFound() const { return textServerFound; }

	/**
	 * Return the text of server search
	 * @return FText
	 */
	FORCEINLINE FText GetTextServerSearch() const { return textServerSearch; }
};
