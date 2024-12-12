// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/MenuInterface.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API AMenuGameMode : public AGameModeBase, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	AMenuGameMode();

};
