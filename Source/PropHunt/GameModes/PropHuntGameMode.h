// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PropHuntGameMode.generated.h"

class APropHuntGameState;
class APropHuntCharacter;

UCLASS(minimalapi)
class APropHuntGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APropHuntGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	void CheckGameStarted();
	void StartGameTimer();
	void ChooseHunterCharacter();
	void SpawnHunter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<APropHuntCharacter> CharacterBlueprint;

private:
	APropHuntGameState* MyGameState;
	FTimerHandle StartGameCountdownHandler;
};



