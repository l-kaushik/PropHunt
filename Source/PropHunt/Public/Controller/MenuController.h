// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"

/**
 * 
 */

class UMenuWidget;
class UPropHuntSubsystem;
UCLASS()
class PROPHUNT_API AMenuController : public APlayerController 
{
	GENERATED_BODY()

public:
	AMenuController();

	virtual void BeginPlay() override;

public:
	void ClientWantsToHost(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch = true);

protected:
	UFUNCTION(Server, Reliable)
	void ClientWantsToHostOnServer(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatc);

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);
private:
	TSubclassOf<UMenuWidget> MenuWidgetBPClassRef;
	UMenuWidget* MenuWidgetRef;

	UPropHuntSubsystem* PropHuntSubsystem;
};
