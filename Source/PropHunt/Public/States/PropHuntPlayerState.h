// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PropHuntPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API APropHuntPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Player Info
	void SetIsReady(bool InIsReady);
	void SetIsHost(bool InIsHost);

	bool GetIsReady() const;
	bool GetIsHost() const;


	// Game Data
	void AddKill();
	void AddAssist();
	void AddDamageDealt(int32 damageDealt);
	void AddDamageTaken(int32 damageTaken);

	int32 GetKills() const;
	int32 GetAssists() const;
	int32 GetDamageGiven() const;
	int32 GetDamageTaken() const;


	// track the current hidden time, and when player move compare with longest hidden time and update accordingly

private:

	// Player Info
	UPROPERTY(Replicated)
	bool m_IsReady;

	UPROPERTY(Replicated)
	bool m_IsHost;

	// Game Data
	UPROPERTY(Replicated)
	int32 m_Kills;

	UPROPERTY(Replicated)
	int32 m_Assists;

	UPROPERTY(Replicated)
	int32 m_DamageGiven;

	UPROPERTY(Replicated)
	int32 m_DamageTaken;

	UPROPERTY(Replicated)
	double m_LongestHiddenTime;
};
