// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/PlayerScoreboardData.h"
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
	APropHuntPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// structs
	FPlayerScoreboardData GetPlayerScoreboardData();
	void SetPlayerData(FPlayerData InPlayerData);

	// Player Info
	void SetIsReady(bool InIsReady);
	void SetIsHost(bool InIsHost);
	void SetUsername(const FString& InUsername);
	void SetProfileImage(FImageData InImageData);

	bool GetIsReady() const;
	bool GetIsHost() const;
	FString GetUsername() const;
	FImageData GetProfileImage() const;


	// Game Data
	void AddKill();
	void AddAssist();
	void AddDamageDealt(int32 damageDealt);
	void AddDamageTaken(int32 damageTaken);
	void SetBestHiddenTimer(double hiddenTime);

	int32 GetKills() const;
	int32 GetAssists() const;
	int32 GetDamageGiven() const;
	int32 GetDamageTaken() const;
	double GetHiddenTime() const;


	// track the current hidden time, and when player move compare with longest hidden time and update accordingly


	// Weapon Data
	void ReduceAmmo();
	void SetCurrentAmmoInMagazine(int32 InAmmo);
	void SetCurrentReserveAmmo(int32 InAmmo);

	int32 GetCurrentAmmoInMagazine() const;
	int32 GetCurrentReserveAmmo() const;
	int32 GetMaxAmmoInMagazine() const;
	int32 GetMaxReserveAmmo() const;

private:

	// Player Info
	UPROPERTY(Replicated)
	bool m_IsReady;

	UPROPERTY(Replicated)
	bool m_IsHost;

	UPROPERTY(Replicated)
	FPlayerScoreboardData m_PlayerScoreboardData;

	// Weapon Data

	UPROPERTY(Replicated)
	int32 m_CurrentAmmoInMagazine;

	UPROPERTY(Replicated)
	int32 m_CurrentReserveAmmo;

	
	static const int32 m_MaxAmmoInMagazine;
	static const int32 m_MaxReserveAmmo;
};
