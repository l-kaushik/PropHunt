// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
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

	// session related functions
	void HostSession(const FName& SessionName, const FString LevelName, int32 NumPublicConnections, bool IsLANMatch = true);
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery = true);

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);
	void OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool Successful);

private:
	virtual void Init() override;

private:
	UPropHuntSubsystem* PropHuntSubsystem;

	bool bIsMultiplayer;
	
};
