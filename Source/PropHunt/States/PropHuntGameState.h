// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PropHuntGameState.generated.h"


class APropHuntPlayerController;
/**
 * 
 */
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

public:

	UPROPERTY(Replicated)
	TArray<APropHuntPlayerController*> PlayerControllerList;

	UPROPERTY(Replicated)
	TArray<APropHuntPlayerController*> HunterList;

	UPROPERTY(Replicated)
	bool bHasGameStarted;

	int32 MinPlayerNum;

	FVector HunterStartLocation;
	bool bIsPropWon;

};
