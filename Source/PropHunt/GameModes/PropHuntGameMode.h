// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/PropHuntGameModeInterface.h"
#include "PropHuntGameMode.generated.h"

class APropHuntGameState;
class APropHuntCharacter;
class APropHuntPlayerController;

UCLASS(minimalapi)
class APropHuntGameMode : public AGameModeBase, public IPropHuntGameModeInterface
{
	GENERATED_BODY()

public:
	APropHuntGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	// interface functions
	virtual void EndTheGame(bool bIsPropWon) override;

protected:
	void StartNextGame();
	void CheckGameStarted();
	void StartGameTimer();
	void ChooseHunterCharacter();
	void SpawnHunter();
	void StartGameLoopTimer();
	void TimerFinishEndGame();
	void SetupInitialWidget(APropHuntPlayerController* HunterController);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<APropHuntCharacter> CharacterBlueprint;

private:
	APropHuntGameState* MyGameState;
	FTimerHandle GameLoopTimer;
};
