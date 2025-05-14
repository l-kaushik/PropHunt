

#pragma once

#include "Structs/MatchData.h"
#include "Algo/Reverse.h"

#include "CoreMinimal.h"
#include "MatchHistoryMap.generated.h"

USTRUCT()
struct FMatchHistoryMap
{
	GENERATED_BODY()

	void Add(const FString& Key, const FMatchData& Value)
	{
		if (!Map.Contains(Key))
		{
			Keys.Add(Key);
		}
		else if (Map[Key] == Value)
		{
			return; // skip redundant updates if FMatchData implements equality
		}
		Map.Add(Key, Value);
	}

	bool Remove(const FString& Key)
	{
		if (!Map.Contains(Key))
		{
			return false;
		}
		Map.Remove(Key);
		Keys.Remove(Key);
		return true;
	}

	FMatchData* GetByIndex(int32 Index)
	{
		if (!Keys.IsValidIndex(Index)) return nullptr;
		return Map.Find(Keys[Index]);
	}

	FMatchData* Find(const FString& Key)
	{
		return Map.Find(Key);
	}

	const TArray<FString> GetOrderedKeys() const
	{
		TArray<FString> NewKeys = Keys;
		Algo::Reverse(NewKeys);
		return NewKeys;
	}

	const TArray<FMatchData> GetOrderedValues() const
	{
		TArray<FMatchData> OrderedValues;
		for (const FString& Key : GetOrderedKeys())
		{
			OrderedValues.Add(Map[Key]);
		}
		return OrderedValues;
	}

	bool IsEmpty() const
	{
		return Keys.IsEmpty();
	}

	UPROPERTY(SaveGame)
	TArray<FString> Keys;

	UPROPERTY(SaveGame)
	TMap<FString, FMatchData> Map;
};
