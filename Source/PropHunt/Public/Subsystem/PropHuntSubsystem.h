// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PropHuntSubsystem.generated.h"

// Delegates for session handling functions
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnCreateSessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnDestroySessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams
(FPHOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnStartSessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnEndSessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnUpdateSessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnRegisterPlayerComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnUnregisterPlayerComplete, bool Successful);

class FNamedOnlineSession;

UCLASS()
class PROPHUNT_API UPropHuntSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPropHuntSubsystem();

	void CreateSession(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch);
	void DestroySession(const FName& SessionName);
	FNamedOnlineSession* FindSessionByName(FName SessionName);
	void FindSessions(int32 MaxSearchResults, bool IsLANQuery);
	void JoinSession(const FName& SessionName, const FOnlineSessionSearchResult& SessionResult);
	void StartSession(const FName& SessionName);
	void EndSession(const FName& SessionName);
	void UpdateSession(const FName& SessionName, const FString& NewLevelName);
	void TryTravelToSession(const FName& SessionName);
	void RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId);
	void UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId);

	FPHOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FPHOnDestroySessionComplete OnDestroySessionCompleteEvent;
	FPHOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FPHOnJoinSessionComplete OnJoinSessionCompleteEvent;
	FPHOnStartSessionComplete OnStartSessionCompleteEvent;
	FPHOnEndSessionComplete OnEndSessionCompleteEvent;
	FPHOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	FPHOnRegisterPlayerComplete OnRegisterPlayerCompleteEvent;
	FPHOnUnregisterPlayerComplete OnUnregisterPlayerCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession(const FName& SessionName);
	void OnStartSessionCompleted(FName SessionName, bool Successful);
	void OnEndSessionCompleted(FName SessionName, bool Successful);
	void OnUpdateSessionCompleted(FName SessionName, bool Successful);
	void OnRegisterPlayerCompleted(FName SessionName, const TArray< FUniqueNetIdRef >& PlayerIds,  bool Successful);
	void OnUnregisterPlayerCompleted(FName SessionName, const TArray< FUniqueNetIdRef >& PlayerIds, bool Successful);

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;

	FOnRegisterPlayersCompleteDelegate RegisterPlayerCompleteDelegate;
	FDelegateHandle RegisterPlayerCompleteDelegateHandle;

	FOnUnregisterPlayersCompleteDelegate UnregisterPlayerCompleteDelegate;
	FDelegateHandle UnregisterPlayerCompleteDelegateHandle;
};
