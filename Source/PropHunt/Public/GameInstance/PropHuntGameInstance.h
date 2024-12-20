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

	// getters
	bool GetIsMultiplayer();
	bool GetIsHost();

	// session related functions
	void HostSession(const FName& SessionName, const FString LevelName, int32 NumPublicConnections, bool IsLANMatch = true);
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery = true);
	void JoinGameSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult);

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);
	void OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool Successful);
	void OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result);

private:
	virtual void Init() override;

private:
	UPropHuntSubsystem* PropHuntSubsystem;

	bool bIsMultiplayer;
	bool bIsHost;
	FName CurrentSessionName;
	
};
