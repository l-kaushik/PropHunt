// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PropHuntSubsystem.generated.h"

// Delegates for session handling functions
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnCreateSessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnDestroySessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams
(FPHOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);

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

	FPHOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FPHOnDestroySessionComplete OnDestroySessionCompleteEvent;
	FPHOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FPHOnJoinSessionComplete OnJoinSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);
	void OnFindSessionsCompleted(bool Successful);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	bool TryTravelToCurrentSession(const FName& SessionName);

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
	
};
