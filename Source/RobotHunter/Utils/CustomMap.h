// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Containers/Array.h>
#include "CustomMap.generated.h"

/**
 * 
 */
USTRUCT()
struct FCustomKeyValuePair
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(VisibleAnywhere)
	FString key = "";
	UPROPERTY(VisibleAnywhere)
	int value = 0;
	
public:
	FCustomKeyValuePair() = default;

	FCustomKeyValuePair(FString _key, int _value)
	{
		key = _key;
		value = _value;
	}
	~FCustomKeyValuePair()
	{

	}

	FORCEINLINE const FString& GetKey() const { return key; }
	FORCEINLINE const int& GetValue() const { return value; }
};


USTRUCT()
struct FCustomMap
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TArray<FCustomKeyValuePair> pairs = {};


public:
	FCustomMap() = default;
	FCustomMap(const TArray<FCustomKeyValuePair>& _pairs)
	{
		pairs = _pairs;
	}

	FORCEINLINE void Add(const FString& _key, const int& _value)
	{
		if (ContainsKey(_key))
			return;
		pairs.Add(FCustomKeyValuePair(_key, _value));
	}

	FORCEINLINE void UpdateValue(const FString& _key,const int& _value)
	{
		for (size_t i = 0; i < pairs.Num(); i++)
		{
			if (pairs[i].GetKey().Equals(_key))
			{
				pairs[i] = FCustomKeyValuePair(pairs[i].GetKey(), pairs[i].GetValue() + _value);
			}
		}
	}


	FORCEINLINE bool IsEmpty()
	{
		return pairs.IsEmpty();
	}
	FORCEINLINE bool ContainsKey(const FString& _key)
	{
		for (size_t i = 0; i < pairs.Num(); i++)
		{
			if (pairs[i].GetKey().Equals(_key))
				return true;
		}
		return false;
	}
	FORCEINLINE void SetValue(const FString& _key, const int& _value)
	{
		for (size_t i = 0; i < pairs.Num(); i++)
		{
			if (pairs[i].GetKey().Equals(_key))
			{
				pairs[i] = FCustomKeyValuePair(pairs[i].GetKey(), _value);
			}
		}
	}

	FORCEINLINE int GetValue(const FString& _key)
	{
		for (size_t i = 0; i < pairs.Num(); i++)
		{
			if (pairs[i].GetKey().Equals(_key))
				return pairs[i].GetValue();
		}
		return NULL;
	}

	FORCEINLINE void Empty()
	{
		pairs.Empty();
	}
	
	FORCEINLINE TArray<FCustomKeyValuePair> Get()
	{
		return pairs;
	}

	int operator[](const FString& _key)
	{
		return GetValue(_key);
	}
	int Num() const
	{
		return pairs.Num();	
	}
};
