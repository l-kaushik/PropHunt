// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/MenuInterface.h"
#include "MenuGameMode.generated.h"

/**
 * 
 */

class APropHuntGameState;
class AMenuController;
class UPropHuntGameInstance;

UCLASS()
class PROPHUNT_API AMenuGameMode : public AGameModeBase, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	AMenuGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* ExistingPlayer) override;
	virtual void ReturnToMainMenuHost() override;

	void StartGame();

private:
	const FUniqueNetId& GetUniqueIdFromController(AMenuController* Player);

private:
	APropHuntGameState* PropHuntGameState;
	UPropHuntGameInstance* PropHuntGameInstance;
};
