// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CustomMathBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTHUNTER_API UCustomMathBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	


public:
#pragma region Trigo

	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetWorldTrigoPointXZ(const float _angle, const float _radius);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetWorldTrigoPointXY(const float _angle, const float _radius);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetWorldTrigoPointZY(const float _angle, const float _radius);

	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetLocalTrigoPointXZ(const float _angle, const float _radius, const FVector& _forward, const FVector& _up);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetLocalTrigoPointXY(const float _angle, const float _radius, const FVector& _forward, const FVector& _right);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetLocalTrigoPointZY(const float _angle, const float _radius, const FVector& _up, const FVector& _right);

	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static float FindAngle(const FVector2D& _dir);

	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static float Angle(const FVector& _a, const FVector& _b);

	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetForwardFromRotation(const FRotator& _rotation);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetRightFromRotation(const FRotator& _rotation);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Trigo")
	static FVector GetUpFromRotation(const FRotator& _rotation);

#pragma endregion

#pragma region Equal

	UFUNCTION(BlueprintPure, Category="Custom|Math|Equal")
	static bool IsVectorSuperiorOrEqual(const FVector& _a, const FVector& _b);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Equal")
	static bool IsVectorInferiorOrEqual(const FVector& _a, const FVector& _b);

	UFUNCTION(BlueprintPure, Category="Custom|Math|Equal")
	static bool IsIntEqual(const int _a, const int _b, const  int _margin);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Equal")
	static bool IsFloatEqual(const float _a, const float _b, const  float _margin);
	
	UFUNCTION(BlueprintCallable, Category="Custom|Math|Operations")
	static void AddAndAssign(UPARAM(ref)
	                         float& Target, float Value);

#pragma endregion

#pragma region Range

	UFUNCTION(BlueprintPure, Category="Custom|Math|Range")
	static bool IsFloatInRange(const float _value, const float _min, const float _max);
	UFUNCTION(BlueprintPure, Category="Custom|Math|Range")
	static bool IsIntInRange(const int _value, const int _min, const int _max);

	UFUNCTION(BlueprintPure, Category = "Custom|Math|Range")
	static bool IsFloatInRangeStrict(const float _value, const float _min, const float _max);
	UFUNCTION(BlueprintPure, Category = "Custom|Math|Range")
	static bool IsIntInRangeStrict(const int _value, const int _min, const int _max);

#pragma endregion
#pragma region Round

	UFUNCTION(BlueprintPure, Category="Custom|Math|Round")
	static float RoundToNumberAfterPoint(const double _a, const int _n);
#pragma endregion


#pragma region normalize
	/**
	 * Normalize the value to get a value between 0 and 1, between 0 and a max value.
	 * @param _valueToNormalize Value to normalize
	 * @param _max Maximum of value
	 * @return A value normalize 0,1
	 */
	UFUNCTION(BlueprintPure, Category="Custom|Math|Normalize")
	static float NormalizeValueZeroMax(const float _valueToNormalize, const float _max);

	/**
	 * Normalize the value to get a value between 0 and 1, between a min and a max value.
	 * @param _valueToNormalize Value to normalize
	 * @param _min Minimum of value
	 * @param _max Maximum of value
	 * @return A value normalize 0,1
	 */
	UFUNCTION(BlueprintPure, Category="Custom|Math|Normalize")
	static float NormalizeValue(float _valueToNormalize, float _min, float _max);
#pragma endregion normalize

	static FVector CustomDistance(const FVector& _v1, const FVector& _v2);

	
	/**
 * @brief Incrémente une valeur entière par une quantité donnée.
 * 
 * @param Value Référence à la valeur à incrémenter.
 * @param IncrementAmount Valeur à ajouter.
 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Math")
	static void IncrementValue(UPARAM(ref) int32& Value, int32 IncrementAmount);

	/**
	 * @brief Incrémente une valeur entière de 1.
	 * 
	 * @param _a Référence à la valeur à incrémenter.
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Math")
	static void IncrementValueByOne(UPARAM(ref) int32& _a);

	/**
	 * @brief Décrémente une valeur entière par une quantité donnée.
	 * 
	 * @param _a Référence à la valeur à décrémenter.
	 * @param _b Valeur à soustraire.
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Math")
	static void DecrementValue(UPARAM(ref) int32& _a, int32 _b);

	/**
	 * @brief Décrémente une valeur entière de 1.
	 * 
	 * @param _a Référence à la valeur à décrémenter.
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Math")
	static void DecrementValueByOne(UPARAM(ref) int32& _a);

	/**
	 * Calcule le modulo positif entre deux entiers et retour le nombre.
	 * 
	 * @param _value Dividende (référence).
	 * @param _modulo Diviseur (référence).
	 * @param _result Resultat
	 * @param _pickAbsolute Get absolute value or not
	 * @return int Le reste de la division (_value mod _modulo).
	 */
	UFUNCTION(BlueprintCallable, Category = "Custom|Math", DisplayName="Modulo")
	static void PositiveModulo(UPARAM(ref) int32& _value,  int32 _modulo, UPARAM(DisplayName = "Result") int32& _result);
};
