// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Interfaces/PropHuntGameModeInterface.h"
#include "PropHuntGameMode.generated.h"

class APropHuntGameState;
class APropHuntCharacter;
class APropCharacter;
class APropHuntPlayerController;

UCLASS(minimalapi)
class APropHuntGameMode : public AGameModeBase, public IPropHuntGameModeInterface
{
	GENERATED_BODY()

public:
	APropHuntGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* ExistingPlayer) override;
	virtual void InitGameState() override;
	void SpawnPlayer(APropHuntPlayerController* PlayerController);
	void HandlePropDeath(APropHuntPlayerController* PlayerController);
	void CleanupPlayerExitFromScoreboard();

	void StartNextGame();
public:
	// interface functions
	virtual void EndTheGame(bool bIsPropWon) override;

protected:
	void CheckGameStarted();
	void StartGameTimer();
	void ChooseHunterCharacter();
	APropHuntCharacter* SpawnCharacter();
	void SpawnHunter(APropHuntPlayerController* HunterController);
	void StartGameLoopTimer();
	void TimerFinishEndGame();
	void SetupInitialWidget();

	template <class T>
	TSubclassOf<T> LoadBlueprint(const FString& BlueprintPath)
	{
		static ConstructorHelpers::FClassFinder<T> BlueprintClass(*BlueprintPath);
		if (BlueprintClass.Succeeded())
		{
			return BlueprintClass.Class;
		}
		return nullptr;
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<APropHuntCharacter> CharacterBlueprint;

	TSubclassOf<APropCharacter> PropCharBlueprintRef;

private:
	APropHuntGameState* MyGameState;
	FTimerHandle GameLoopTimer;
};
