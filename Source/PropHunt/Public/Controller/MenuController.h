// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../Interfaces/MenuInterface.h"
#include "MenuController.generated.h"

/**
 * 
 */

class UMenuWidget;
UCLASS()
class PROPHUNT_API AMenuController : public APlayerController, public IMenuInterface
{
	GENERATED_BODY()

public:
	AMenuController();

	virtual void BeginPlay() override;

public:
	// interface functions
	virtual void HostServer(const FString& ServerName, const int32& NumberOfPlayers) override;

private:
	UFUNCTION(Server, Reliable)
	void RequestHostServer(const FString& ServerName, const int32& NumberOfPlayers);

private:
	TSubclassOf<UMenuWidget> MenuWidgetBPClassRef;

	UMenuWidget* MenuWidgetRef;
	
};
