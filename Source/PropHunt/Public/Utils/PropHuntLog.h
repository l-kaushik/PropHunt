// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define UE_LOG_NON_SHIP(CategoryName, Verbosity, Format, ...) \
	if(!UE_BUILD_SHIPPING) \
	{ \
		UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__); \
	} \

	

DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntGlobal, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntSubsystem, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntGameInstance, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntMenuController, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntController, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntMenuGameMode, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntGameMode, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntWidget, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropHuntCharacter, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPropCharacter, Log, All);