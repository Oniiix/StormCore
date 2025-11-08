

#pragma once

#include "CoreMinimal.h"
#include "Components/VolumetricCloudComponent.h"
#include "CustomVolumetricCloudComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Rendering, collapsecategories, hidecategories = (Object, Mobility, Activation, "Components|Activation"), editinlinenew, meta = (BlueprintSpawnableComponent))
class ROBOTHUNTER_API UCustomVolumetricCloudComponent : public USceneComponent
{
	GENERATED_BODY()
	
	/** The altitude at which the cloud layer starts. (kilometers above the ground) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Layer", meta = (UIMin = 0.0f, UIMax = 20.0f, SliderExponent = 2.0, AllowPrivateAccess))
	float LayerBottomAltitude;

	/** The height of the the cloud layer. (kilometers above the layer bottom altitude) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Layer", meta = (UIMin = 0.1f, UIMax = 20.0f, ClampMin = 0.1, SliderExponent = 2.0, AllowPrivateAccess))
	float LayerHeight;

	/** The maximum distance of the volumetric surface, i.e. cloud layer upper and lower bound, before which we will accept to start tracing. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Layer", meta = (UIMin = 100.0f, UIMax = 500.0f, ClampMin = 1.0f, SliderExponent = 2.0, AllowPrivateAccess))
	float TracingStartMaxDistance;

	/** The distance from which the tracing will start. This is useful when the camera for instance is inside the layer of cloud. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Layer", meta = (UIMin = 0.0f, UIMax = 100.0f, ClampMin = 0.0f, SliderExponent = 3.0, AllowPrivateAccess))
	float TracingStartDistanceFromCamera;

	/** Mode to select how the tracing max distance should be interpreted. DistanceFromPointOfView is useful to avoid the top of the cloud layer to be clipped when TracingMaxDistance is shorten for performance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Layer", meta = (AllowPrivateAccess))
	EVolumetricCloudTracingMaxDistanceMode TracingMaxDistanceMode;

	/** The maximum distance that will be traced inside the cloud layer. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Layer", meta = (UIMin = 1.0f, UIMax = 500.0f, ClampMin = 0.1f, SliderExponent = 2.0, AllowPrivateAccess))
	float TracingMaxDistance;

	/** The planet radius used when there is not SkyAtmosphere component present in the scene. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Planet", meta = (UIMin = 100.0f, UIMax = 7000.0f, ClampMin = 0.1, ClampMax = 10000.0f, AllowPrivateAccess))
	float PlanetRadius;

	/**
	 * The ground albedo used to light the cloud from below with respect to the sun light and sky atmosphere.
	 * This is only used by the cloud material when the 'Volumetric Advanced' node have GroundContribution enabled.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Planet", meta = (HideAlphaChannel, AllowPrivateAccess))
	FColor GroundAlbedo;

	/** The material describing the cloud volume. It must be a Volume domain material. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Material", meta = (AllowPrivateAccess))
	TObjectPtr<UMaterialInterface> Material;

	/** Whether to apply atmosphere transmittance per sample, instead of using the light global transmittance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (AllowPrivateAccess))
	uint32 bUsePerSampleAtmosphericLightTransmittance : 1;
	// bUsePerSampleAtmosphericLightTransmittance is there on the cloud component and not on the light because otherwise we would need optimization permutations of the cloud shader.
	// And this for the two atmospheric lights ON or OFF. Keeping it simple for now because this changes the look of the cloud, so it is an art/look decision.

	/** Occlude the sky light contribution at the bottom of the cloud layer. This is a fast approximation to sky lighting being occluded by cloud without having to trace rays or sample AO texture. Ignored if the cloud material explicitely sets the ambient occlusion value. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = 0.0f, UIMax = 1.0f, ClampMin = 0.0f, ClampMax = 1.0f, AllowPrivateAccess))
	float SkyLightCloudBottomOcclusion;

	/**
	 * Scale the tracing sample count in primary views. Quality level scalability CVARs affect the maximum range.
	 * The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.ViewRaySampleCountMax'.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = "0.05", UIMax = "8", ClampMin = "0.05", SliderExponent = 1.0, AllowPrivateAccess))
	float ViewSampleCountScale;
	/**
	 * Scale the tracing sample count in reflection views. Quality level scalability CVARs affect the maximum range.
	 * The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.ReflectionRaySampleMaxCount'.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = "0.05", UIMax = "8", ClampMin = "0.05", SliderExponent = 1.0, AllowPrivateAccess))
	float ReflectionViewSampleCountScaleValue;
	UPROPERTY()
	float ReflectionViewSampleCountScale_DEPRECATED;
	UPROPERTY()
	float ReflectionSampleCountScale_DEPRECATED;

	/**
	 * Scale the shadow tracing sample count in primary views, only used with Advanced Output ray marched shadows. Quality level scalability CVARs affect the maximum range.
	 * The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.Shadow.ViewRaySampleMaxCount'.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = "0.05", UIMax = "8", ClampMin = "0.05", SliderExponent = 1.0, AllowPrivateAccess))
	float ShadowViewSampleCountScale;
	/**
	 * Scale the shadow tracing sample count in reflection views, only used with Advanced Output ray marched shadows. Quality level scalability CVARs affect the maximum range.
	 * The sample count resolution is still clamped according to scalability setting to 'r.VolumetricCloud.Shadow.ReflectionRaySampleMaxCount'.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = "0.05", UIMax = "8", ClampMin = "0.05", SliderExponent = 1.0, AllowPrivateAccess))
	float ShadowReflectionViewSampleCountScaleValue;
	UPROPERTY()
	float ShadowReflectionViewSampleCountScale_DEPRECATED;
	UPROPERTY()
	float ShadowReflectionSampleCountScale_DEPRECATED;

	/**
	 * The shadow tracing distance in kilometers, only used with Advanced Output ray marched shadows.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = "0.1", UIMax = "50", ClampMin = "0.01", SliderExponent = 3.0, AllowPrivateAccess))
	float ShadowTracingDistance;

	/**
	 * When the mean transmittance is below this threshold, we stop tracing. This is a good way to reduce the ray marched sample count, and thus to increase performance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloud Tracing", meta = (UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0", SliderExponent = 5.0, AllowPrivateAccess))
	float StopTracingTransmittanceThreshold;

	/** Specify the aerial perspective start distance on cloud for Rayleigh scattering only. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Art Direction", meta = (UIMin = 0.0, UIMax = 100.0, ClampMin = 0.0, SliderExponent = 2.0, AllowPrivateAccess))
	float AerialPespectiveRayleighScatteringStartDistance;
	/** Specify the distance over which the Rayleigh scattering will linearly ramp up to full effect. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Art Direction", meta = (UIMin = 0.0, UIMax = 100.0, ClampMin = 0.0, SliderExponent = 2.0, AllowPrivateAccess))
	float AerialPespectiveRayleighScatteringFadeDistance;
	/** Specify the aerial perspective start distance on cloud for Mie scattering only. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Art Direction", meta = (UIMin = 0.0, UIMax = 100.0, ClampMin = 0.0, SliderExponent = 2.0, AllowPrivateAccess))
	float AerialPespectiveMieScatteringStartDistance;
	/** Specify the distance over which the Rayleigh scattering will linearly ramp up to full effect. (kilometers) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, interp, Category = "Art Direction", meta = (UIMin = 0.0, UIMax = 100.0, ClampMin = 0.0, SliderExponent = 2.0, AllowPrivateAccess))
	float AerialPespectiveMieScatteringFadeDistance;

	/** If this is True, this primitive will render black with an alpha of 0, but all secondary effects (shadows, reflections, indirect lighting) remain. This feature required the project setting "Enable alpha channel support in post processing". */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Rendering, Interp, meta = (AllowPrivateAccess))
	uint8 bHoldout : 1;

	/** If true, this component will be rendered in the main pass (basepass, transparency) */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = Rendering, meta = (AllowPrivateAccess))
	uint8 bRenderInMainPass : 1;


public:
	UCustomVolumetricCloudComponent(const FObjectInitializer& ObjectInitializer);

	 UFUNCTION(BlueprintCallable) FORCEINLINE void SetAerialPespectiveRayleighScatteringStartDistance(const float _value) { AerialPespectiveRayleighScatteringStartDistance = _value; }
	 UFUNCTION(BlueprintCallable) FORCEINLINE void SetAerialPespectiveRayleighScatteringFadeDistance(const float _value) { AerialPespectiveRayleighScatteringFadeDistance = _value; }
	 UFUNCTION(BlueprintCallable) FORCEINLINE void SetAerialPespectiveMieScatteringStartDistance(const float _value) { AerialPespectiveMieScatteringStartDistance = _value; }
	 UFUNCTION(BlueprintCallable) FORCEINLINE void SetAerialPespectiveMieScatteringFadeDistance(const float _value) { AerialPespectiveMieScatteringFadeDistance = _value; }
private:
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetLayerBottomAltitude(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetLayerHeight(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetTracingStartMaxDistance(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetTracingStartDistanceFromCamera(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetTracingMaxDistance(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetPlanetRadius(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetGroundAlbedo(FColor NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering", meta = (DisplayName = "Set Use Per Sample Atmospheric Light Transmittance"))
	void SetbUsePerSampleAtmosphericLightTransmittance(bool NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetSkyLightCloudBottomOcclusion(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetViewSampleCountScale(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetReflectionViewSampleCountScale(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetShadowViewSampleCountScale(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetShadowReflectionViewSampleCountScale(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetShadowTracingDistance(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetStopTracingTransmittanceThreshold(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetMaterial(UMaterialInterface* NewValue);

	UMaterialInterface* GetMaterial() const { return Material; }

	// Deprecated functions but still valid because they forward data correctly.
	UE_DEPRECATED(5.0, "This function has been replaced by SetReflectionViewSampleCountScale.")
		UFUNCTION(BlueprintCallable, Category = "Rendering", meta = (DeprecatedFunction, DeprecationMessage = "This function has been replaced by SetReflectionViewSampleCountScale."))
	void SetReflectionSampleCountScale(float NewValue);
	UE_DEPRECATED(5.0, "This function has been replaced by SetShadowReflectionViewSampleCountScale.")
		UFUNCTION(BlueprintCallable, Category = "Rendering", meta = (DeprecatedFunction, DeprecationMessage = "This function has been replaced by SetShadowReflectionViewSampleCountScale."))
	void SetShadowReflectionSampleCountScale(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetHoldout(bool bNewHoldout);

	UFUNCTION(BlueprintCallable, Category = "Rendering")
	void SetRenderInMainPass(bool bValue);



	// Those values should never be changed wihtout data conversion, that in order to maintain performance in case default values are used.
	static constexpr float BaseViewRaySampleCount = 96.0f;
	static constexpr float BaseShadowRaySampleCount = 10.0f;
	// Those values are part of a data conversion and should never be changed. CVars and component sample count controls should be enough.
	static constexpr float OldToNewReflectionViewRaySampleCount = 10.0f / BaseViewRaySampleCount;
	static constexpr float OldToNewReflectionShadowRaySampleCount = 3.0f / BaseShadowRaySampleCount;

private:

	FVolumetricCloudSceneProxy* VolumetricCloudSceneProxy;

};
