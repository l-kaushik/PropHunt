// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "../Widget/MainHud.h"
#include "PropHuntPlayerController.h"


APropHuntPlayerController::APropHuntPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> MainHudBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_MainHud"));
	if (MainHudBPClass.Succeeded()) {
		MainHudBPClassRef = MainHudBPClass.Class;
	}
}

void APropHuntPlayerController::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Working"));

	/*
	* In "Listen server architecture"
	* HasAuthority() && IsLocalPlayerController() --> this line checks, if listen server is a client also, if it is then create UI for it
	* the || !HasAuthority() -->this line create UI for remote player
	* 
	* Hence, it ensure all client gets UI
	*/

	if ((HasAuthority() && IsLocalPlayerController()) || !HasAuthority()) {
		if (!MainHudBPClassRef) return;

		MainHudRef = CreateWidget<UMainHud>(this, MainHudBPClassRef);
		check(MainHudRef);
		MainHudRef->AddToPlayerScreen();
	}
}

void APropHuntPlayerController::SetupPropWidget(bool bIsProp)
{
	HandleHudWidgetOnClient(bIsProp);
}

void APropHuntPlayerController::TrySetupPropWidget(bool bIsProp)
{
	if (IPropHuntControllerInterface* PropHuntInterface = Cast<IPropHuntControllerInterface>(this))
	{
		PropHuntInterface->SetupPropWidget(bIsProp);
	}
}

void APropHuntPlayerController::HandleHudWidgetOnClient_Implementation(bool bIsProp)
{
	MainHudRef->SetupPropWidget(bIsProp);
}
