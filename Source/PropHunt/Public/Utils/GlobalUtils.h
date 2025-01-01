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
	static TSubclassOf<T> LoadWidgetBlueprint(const FString WidgetPath)
	{
		static ConstructorHelpers::FClassFinder<T> WidgetBPClass(*WidgetPath);
		return WidgetBPClass.Succeeded() ? WidgetBPClass.Class : nullptr;
	}
};
