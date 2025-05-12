// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/MapInfo.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PropHuntGameInstance.generated.h"

struct FMapInfo;
class UPropHuntSubsystem;
class FOnlineSessionSearchResult;

UCLASS()
class PROPHUNT_API UPropHuntGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// setters
	void SetPlayerNum(int32 InPlayerNum);
	void SetLastDisconnectReason(const FText& ReturnReason);
	void SetMapInfo(const FMapInfo& InMapInfo);

	// getters
	int32 GetPlayerNum() const;
	bool GetIsMultiplayer() const;
	bool GetIsHost() const;
	const FMapInfo& GetMapInfo() const;
	FString GetLastDisconnectReason() const;

	// session related functions
	void HostSession(const FName& SessionName, const FMapInfo& MapInfo, int32 NumPublicConnections, bool IsLANMatch = true);
	void StartSession();
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery = true);
	void StartFindSessions();
	void JoinGameSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult);
	void RegisterPlayer(const FUniqueNetId& PlayerId);
	void UnregisterPlayer(const FUniqueNetId& PlayerId);
	void DestroySession();
	virtual void ReturnToMainMenu() override;

	void QuitGameCleanup();

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);
	void OnStartSessionCompleted(bool Successful);
	void OnFindSessionsCompleted(const TArray<FOnlineSessionSearchResult>& SearchResults, bool Successful);
	void OnJoinSessionCompleted(EOnJoinSessionCompleteResult::Type Result);
	void OnRegisterPlayerCompleted(bool Successful);
	void OnUnregisterPlayerCompleted(bool Successful);
	void onDestroySessionCompleted(bool Successful);

private:
	virtual void Init() override;

	UFUNCTION()
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	UFUNCTION()
	void HandleTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString);
	void HandleFailureCleanups();

	bool IsCurrentSessionName(const FString& CalleInfo);
	class AMenuController* GetPlayerController();

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
	FMapInfo CurrentMapInfo;
	FName CurrentSessionName;
	int32 PlayerNum;
	FString LastDisconnectReason;
	FFindSessionSettings FindSessionSettings;

	FTimerHandle FindSessionTimerHandle;
	void StartFindSessionLoop();
	void StopFindSessionLoop();
};
