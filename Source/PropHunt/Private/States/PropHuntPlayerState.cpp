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
	DOREPLIFETIME(APropHuntPlayerState, m_Kills);
	DOREPLIFETIME(APropHuntPlayerState, m_Assists);
	DOREPLIFETIME(APropHuntPlayerState, m_DamageGiven);
	DOREPLIFETIME(APropHuntPlayerState, m_DamageTaken);
	DOREPLIFETIME(APropHuntPlayerState, m_LongestHiddenTime);

	// Weapon Data
	DOREPLIFETIME(APropHuntPlayerState, m_CurrentAmmoInMagazine)
	DOREPLIFETIME(APropHuntPlayerState, m_CurrentReserveAmmo)
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
	m_Username = InUsername;
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
	return m_Username;
}

void APropHuntPlayerState::AddKill()
{
	m_Kills++;
}

void APropHuntPlayerState::AddAssist()
{
	m_Assists++;
}

void APropHuntPlayerState::AddDamageDealt(int32 damageDealt)
{
	m_DamageGiven += damageDealt;
}

void APropHuntPlayerState::AddDamageTaken(int32 damageTaken)
{
	m_DamageTaken += damageTaken;
}

void APropHuntPlayerState::SetBestHiddenTimer(double hiddenTime)
{
	m_LongestHiddenTime = hiddenTime;
}

int32 APropHuntPlayerState::GetKills() const
{
	return m_Kills;
}

int32 APropHuntPlayerState::GetAssists() const
{
	return m_Assists;
}

int32 APropHuntPlayerState::GetDamageGiven() const
{
	return m_DamageGiven;
}

int32 APropHuntPlayerState::GetDamageTaken() const
{
	return m_DamageTaken;
}

double APropHuntPlayerState::GetHiddenTime() const
{
	return m_LongestHiddenTime;
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

