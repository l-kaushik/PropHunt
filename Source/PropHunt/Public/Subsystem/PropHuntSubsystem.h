// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PropHuntSubsystem.generated.h"

// Delegates for session handling functions
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnCreateSessionComplete, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnDestroySessionComplete, bool Successful);


UCLASS()
class PROPHUNT_API UPropHuntSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPropHuntSubsystem();

	void CreateSession(FName& SessionName, FString& LevelName, int32& NumPublicConnections, bool IsLANMatch);
	void DestroySession(FName& SessionName);

	FPHOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FPHOnDestroySessionComplete OnDestroySessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);
	void OnDestroySessionCompleted(FName SessionName, bool Successful);

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
};
