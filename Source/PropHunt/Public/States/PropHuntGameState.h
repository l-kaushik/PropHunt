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

	// Getters
	TArray<APropHuntPlayerController*>& GetPlayerControllerList();
	const TArray<AMenuController*>& GetMenuPlayerControllerList() const;
	TArray<APropHuntPlayerController*>& GetHunterList();
	const TArray<APropHuntPlayerState*>& GetPlayerStates() const;
	bool GetHasGameStarted() const;
	bool GetIsPropWon() const;
	int32 GetMinPlayerNum() const;

	// Setters
	void AddPlayerController(APropHuntPlayerController* NewController);
	void AddMenuController(AMenuController* NewController);
	void RemoveMenuController(AMenuController* ExistingController);
	void AddHunter(APropHuntPlayerController* NewHunter);
	void SetHasGameStarted(bool InHasGameStarted);
	void SetIsPropWon(bool InIsPropWon);
	void SetMinPlayerNum(int32 InMinPlayerNum);

public:
	FOnPlayerListUpdated OnPlayerListUpdated;

	FVector HunterStartLocation;
	static const float GAME_TIME_IN_SECONDS;

protected:
	UFUNCTION()
	void OnRep_PlayerStates();

private:
	UPROPERTY(Replicated)
	TArray<APropHuntPlayerController*> PlayerControllerList;

	UPROPERTY(Replicated)
	TArray<AMenuController*> MenuPlayerControllerList;

	UPROPERTY(Replicated)
	TArray<APropHuntPlayerController*> HunterList;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerStates)
	TArray<APropHuntPlayerState*> PlayerStates;

	UPROPERTY(Replicated)
	bool bHasGameStarted;

	UPROPERTY(Replicated)
	int32 MinPlayerNum;

	UPROPERTY(Replicated)
	bool bIsPropWon;
};
