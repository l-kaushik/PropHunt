// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PropHuntSubsystem.generated.h"

// Delegates for session handling functions
DECLARE_MULTICAST_DELEGATE_OneParam(FPHOnCreateSessionComplete, bool Successful);


UCLASS()
class PROPHUNT_API UPropHuntSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPropHuntSubsystem();

	void CreateSession(FName& SessionName, FString& LevelName, int32& NumPublicConnections, bool IsLANMatch);

	FPHOnCreateSessionComplete OnCreateSessionCompleteEvent;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

private:
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
};
