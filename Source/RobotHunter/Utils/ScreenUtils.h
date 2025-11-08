// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Widgets/Layout/Anchors.h"
#include "ScreenUtils.generated.h"


UENUM()
enum class EMarginSide : uint8
{
	Left, Top
};
/**
 * 
 */
UCLASS(HideDropdown)
class ROBOTHUNTER_API UScreenUtils : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Find the margins of a zone from a reference screen size
	 * @param _reference Size reference (size of screen)
	 * @param _size position of vector 2D
	 * @param _side side of margin
	 * @return margin divided by 2
	 */
	static float GetMargin(const FVector2D& _reference, const FVector2D& _size, const EMarginSide _side);
	
	/**
	 * Get the size of viewport screen
	 * @return Vector 2D of size or 1920x1080 if viewport is null
	 */
	static FVector2D GetScreenViewport();
			
	/**
	 * Check if vector 1
	 * @param _vector vector to test
	 * @param _reference vector of reference 
	 * @return true of false
	 */
	static bool VectorContainsInTargetZone(const FVector2D& _vector, const FVector2D& _reference);

	/**
	 * Get the anchors of target zone from screen reference
	 * @param _screen screen size
	 * @param _marginLR margin left and right
	 * @param _marginTB margin top and bottom
	 * @return A struct of anchors size
	 */
	FAnchors static GetAnchorsTargetZone(FVector2D _screen, float _marginLR, float _marginTB);

};
