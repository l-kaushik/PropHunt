// Fill out your copyright notice in the Description page of Project Settings.


#include "States/PropHuntPlayerState.h"

#include "Net/UnrealNetwork.h"

const int32 APropHuntPlayerState::m_MaxAmmoInMagazine = 30;
const int32 APropHuntPlayerState::m_MaxReserveAmmo = 180;

APropHuntPlayerState::APropHuntPlayerState()
{
	m_CurrentAmmoInMagazine = m_MaxAmmoInMagazine;
	m_CurrentReserveAmmo = m_MaxReserveAmmo;
}

void APropHuntPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Player Info
	DOREPLIFETIME(APropHuntPlayerState, m_IsReady)
	DOREPLIFETIME(APropHuntPlayerState, m_IsHost)

	// Game Data
	DOREPLIFETIME(APropHuntPlayerState, m_PlayerScoreboardData);

	// Weapon Data
	DOREPLIFETIME(APropHuntPlayerState, m_CurrentAmmoInMagazine)
	DOREPLIFETIME(APropHuntPlayerState, m_CurrentReserveAmmo)
}

FPlayerScoreboardData APropHuntPlayerState::GetPlayerScoreboardData()
{
	return m_PlayerScoreboardData;
}

void APropHuntPlayerState::SetPlayerData(FPlayerData InPlayerData)
{
	m_PlayerScoreboardData.PlayerData = InPlayerData;
}

void APropHuntPlayerState::SetIsReady(bool InIsReady)
{
	m_IsReady = InIsReady;
}

void APropHuntPlayerState::SetIsHost(bool InIsHost)
{
	m_IsHost = InIsHost;
}

void APropHuntPlayerState::SetUsername(const FString& InUsername)
{
	m_PlayerScoreboardData.PlayerData.Username = InUsername;
}

void APropHuntPlayerState::SetProfileImage(FImageData InImageData)
{
	m_PlayerScoreboardData.PlayerData.ProfileImage = InImageData;
}

bool APropHuntPlayerState::GetIsReady() const
{
	return m_IsReady;
}

bool APropHuntPlayerState::GetIsHost() const
{
	return m_IsHost;
}

FString APropHuntPlayerState::GetUsername() const
{
	return m_PlayerScoreboardData.PlayerData.Username;
}

FImageData APropHuntPlayerState::GetProfileImage() const
{
	return m_PlayerScoreboardData.PlayerData.ProfileImage;
}

const FPlayerData& APropHuntPlayerState::GetPlayerData() const
{
	return m_PlayerScoreboardData.PlayerData;
}

void APropHuntPlayerState::AddKill()
{
	m_PlayerScoreboardData.kills++;
}

void APropHuntPlayerState::AddAssist()
{
	m_PlayerScoreboardData.Assists++;
}

void APropHuntPlayerState::AddDamageDealt(int32 damageDealt)
{
	m_PlayerScoreboardData.DamageGiven += damageDealt;
}

void APropHuntPlayerState::AddDamageTaken(int32 damageTaken)
{
	m_PlayerScoreboardData.DamageTaken += damageTaken;
}

void APropHuntPlayerState::SetBestHiddenTimer(double hiddenTime)
{
	m_PlayerScoreboardData.HiddenTime = hiddenTime;
}

int32 APropHuntPlayerState::GetKills() const
{
	return m_PlayerScoreboardData.kills;
}

int32 APropHuntPlayerState::GetAssists() const
{
	return m_PlayerScoreboardData.Assists;
}

int32 APropHuntPlayerState::GetDamageGiven() const
{
	return m_PlayerScoreboardData.DamageGiven;
}

int32 APropHuntPlayerState::GetDamageTaken() const
{
	return m_PlayerScoreboardData.DamageTaken;
}

double APropHuntPlayerState::GetHiddenTime() const
{
	return m_PlayerScoreboardData.HiddenTime;
}

void APropHuntPlayerState::ReduceAmmo()
{
	m_CurrentAmmoInMagazine--;
}

void APropHuntPlayerState::SetCurrentAmmoInMagazine(int32 InAmmo)
{
	m_CurrentAmmoInMagazine = InAmmo;
}

void APropHuntPlayerState::SetCurrentReserveAmmo(int32 InAmmo)
{
	m_CurrentReserveAmmo = InAmmo;
}

int32 APropHuntPlayerState::GetCurrentAmmoInMagazine() const
{
	return m_CurrentAmmoInMagazine;
}

int32 APropHuntPlayerState::GetCurrentReserveAmmo() const
{
	return m_CurrentReserveAmmo;
}

int32 APropHuntPlayerState::GetMaxAmmoInMagazine() const
{
	return m_MaxAmmoInMagazine;
}

int32 APropHuntPlayerState::GetMaxReserveAmmo() const
{
	return m_MaxReserveAmmo;
}

