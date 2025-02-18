// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PropHuntGameInstance.generated.h"

class UPropHuntSubsystem;
class FOnlineSessionSearchResult;

UCLASS()
class PROPHUNT_API UPropHuntGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// setters
	void SetPlayerNum(int32 InPlayerNum);

	// getters
	int32 GetPlayerNum() const;
	bool GetIsMultiplayer() const;
	bool GetIsHost() const;

	// session related functions
	void HostSession(const FName& SessionName, const FString LevelName, int32 NumPublicConnections, bool IsLANMatch = true);
	void StartSession();
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery = true);
	void StartFindSessions();
	void JoinGameSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult);
	void RegisterPlayer(const FUniqueNetId& PlayerId);
	void UnregisterPlayer(const FUniqueNetId& PlayerId);

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);
	void OnStartSessionCompleted(bool Successful);
	void OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool Successful);
	void OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result);
	void OnRegisterPlayerCompleted(bool Successful);
	void OnUnregisterPlayerCompleted(bool Successful);

private:
	virtual void Init() override;
	bool IsCurrentSessionName(const FString& CalleInfo);

private:
	struct FFindSessionSettings
	{
		int32 MaxSearchResults;
		bool bIsLanQuery;
		bool bIsFindingSession;

		FFindSessionSettings()
			: MaxSearchResults(10), bIsLanQuery(true), bIsFindingSession(false) {
		}
	};

private:
	UPropHuntSubsystem* PropHuntSubsystem;

	bool bIsMultiplayer;
	bool bIsHost;
	FName CurrentSessionName;
	int32 PlayerNum;
	FFindSessionSettings FindSessionSettings;

	FTimerHandle FindSessionTimerHandle;
	void StartFindSessionLoop();
	void StopFindSessionLoop();
};
