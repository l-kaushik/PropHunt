// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROPHUNT_API GlobalUtils
{
public:
	GlobalUtils();
	~GlobalUtils();

public:
	template<typename T>
	static TSubclassOf<T> LoadBlueprint(const FString BlueprintPath)
	{
		static ConstructorHelpers::FClassFinder<T> BPClass(*BlueprintPath);
		return BPClass.Succeeded() ? BPClass.Class : nullptr;
	}
};
