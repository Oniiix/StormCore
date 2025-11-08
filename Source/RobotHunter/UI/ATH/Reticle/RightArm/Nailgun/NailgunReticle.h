// Objectif 3D Copyright

#pragma once

#include "CoreMinimal.h"
#include "RobotHunter/Game/GPE/Members/RightArmActors/NailGun/NailGunActor.h"
#include "RobotHunter/UI/ATH/Reticle/Reticle.h"
#include "NailgunReticle.generated.h"

/**
 * 
 */

#define DEBUG_NAILGUN_RETICULE true

UCLASS()
class ROBOTHUNTER_API UNailgunReticle final : public UReticle
{
	GENERATED_BODY()

#pragma region properties

protected:
	UPROPERTY()
	TObjectPtr<ANailGunActor> nailGunActor = nullptr;
	/// Radius of the circle defining the crosshair
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float radius = 25.f;
	/// Maximum number of charging phases of the NailGun
	//UPROPERTY(EditAnywhere, Category = "Parameter")
	int maxCharge;
	/// Thickness of the segments drawn for the crosshair
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float segmentThickness = 5.0f;
	/// Angle of spacing between each segment of the crosshair
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float spaceAngle = 10.0f;
	/// Starting angle (rotation) of the circle where the segments are located (in degrees).
	UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = "0", ClampMax = "360"))
	int startAngleCircle = 270;

private:
	/// Current fill percentage of the crosshair (represents progress based on the weapon's charging phases).
	float fillPercentage = 0.f;
	/// Current charging phase of the weapon. The value starts at -1 (uncharged).
	int32 currentPhase = -1;

#pragma endregion

#pragma region methods

protected:
	virtual void InitWithDelay() override;
	/** 
	 * Method called when the weapon's charging phase is updated.
	 * Updates the internal variables of the crosshair (`currentPhase` and `fillPercentage`)
	 * and triggers a graphical refresh of the crosshair.
	 */
	UFUNCTION()
	void OnUpdateCounted(float _float);
	/**
	 * Registers events related to the "NailGun."
	 * In particular, links the weapon's phase updates to the method "OnPhaseUpdated."
	 */
	virtual void RegisterEvents() override;

private:
	/**
	 * Main method used to draw the crosshair in the user interface.
	 * This method calculates and draws the circular segments of the crosshair based on the charging state.
	 * 
	 * @return The next available LayerId after completing the drawing.
	 */
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	                          const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	                          const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	/** 
	 * Draws a single segment of the crosshair as a circular arc between a starting angle and an ending angle.
	 * The segments are dynamically computed and drawn.
	 *
	 * @param OutDrawElements List of graphic elements where the segment will be added.
	 * @param AllottedGeometry Geometry of the widget being used.
	 * @param Center Coordinates of the center of the crosshair within the widget.
	 * @param _startAngle Starting angle for the segment (in radians).
	 * @param _endAngle Ending angle for the segment (in radians).
	 * @param LayerId Graphic layer where the segment is drawn.
	 */
	void DrawSegment(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
	                 const FVector2D& Center, float _startAngle, float _endAngle, int32 LayerId) const;
	/**
	 * Draws a straight line between two points.
	 * This method is used to construct each segment of the crosshair.
	 * 
	 * @param OutDrawElements List of graphic elements where the line will be added.
	 * @param AllottedGeometry Geometry of the widget being used.
	 * @param _start Start point of the line.
	 * @param _end End point of the line.
	 * @param Color Color of the line being drawn.
	 * @param LineThickness Thickness of the line being drawn.
	 * @param LayerId Graphic layer where the line is drawn.
	 */
	void DrawLine(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry, const FVector2D& _start,
	              const FVector2D& _end, const FLinearColor& Color, float LineThickness, int32 LayerId) const;
#pragma endregion
};
