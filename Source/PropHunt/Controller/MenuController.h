// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuController.generated.h"

/**
 * 
 */

class UMenuWidget;
UCLASS()
class PROPHUNT_API AMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	AMenuController();

	virtual void BeginPlay() override;

private:
	TSubclassOf<UMenuWidget> MenuWidgetBPClassRef;

	UMenuWidget* MenuWidgetRef;
	
};
