// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PropHuntGameInstance.generated.h"

class UPropHuntSubsystem;

UCLASS()
class PROPHUNT_API UPropHuntGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void HostSession(const FName& SessionName, const FString LevelName, int32 NumPublicConnections, bool IsLANMatch);

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);

private:
	virtual void Init() override;

private:
	UPropHuntSubsystem* PropHuntSubsystem;
	
};
