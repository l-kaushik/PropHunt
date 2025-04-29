// Fill out your copyright notice in the Description page of Project Settings.


#include "States/PropHuntPlayerState.h"

#include "Net/UnrealNetwork.h"

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
}

void APropHuntPlayerState::SetIsReady(bool InIsReady)
{
	m_IsReady = InIsReady;
}

void APropHuntPlayerState::SetIsHost(bool InIsHost)
{
	m_IsHost = InIsHost;
}

bool APropHuntPlayerState::GetIsReady() const
{
	return m_IsReady;
}

bool APropHuntPlayerState::GetIsHost() const
{
	return m_IsHost;
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

