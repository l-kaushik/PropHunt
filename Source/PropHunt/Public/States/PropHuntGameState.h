// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PropHuntGameState.generated.h"




class APropHuntPlayerController;
class AMenuController;
class APropHuntPlayerState;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerListUpdated, const TArray<APropHuntPlayerState*>& PlayerStates);

UCLASS()
class PROPHUNT_API APropHuntGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	APropHuntGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddPlayerController(APropHuntPlayerController* NewController);
	TArray<APropHuntPlayerController*> GetPlayerControllerList() const;

	void AddHunter(APropHuntPlayerController* NewHunter);

	void AddMenuController(AMenuController* NewController);
public:

	UPROPERTY(Replicated)
	TArray<APropHuntPlayerController*> PlayerControllerList;

	UPROPERTY(Replicated)
	TArray<AMenuController*> MenuPlayerControllerList;

	UPROPERTY(Replicated)
	TArray<APropHuntPlayerController*> HunterList;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerStates)
	TArray<APropHuntPlayerState*> PlayerStates;

	FOnPlayerListUpdated OnPlayerListUpdated;

	UPROPERTY(Replicated)
	bool bHasGameStarted;

	int32 MinPlayerNum;

	FVector HunterStartLocation;
	bool bIsPropWon;
	static const float GAME_TIME_IN_SECONDS;

protected:
	UFUNCTION()
	void OnRep_PlayerStates();
};
