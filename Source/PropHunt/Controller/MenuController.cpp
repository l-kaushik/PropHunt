// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuController.h"
#include "../Widget/MenuWidget.h"
#include "../GameModes/MenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AMenuController::AMenuController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidgetBPClass(TEXT("/Game/ThirdPerson/Widgets/WB_Menu"));
	if (MenuWidgetBPClass.Succeeded())
	{
		MenuWidgetBPClassRef = MenuWidgetBPClass.Class;
	}
}

void AMenuController::BeginPlay()
{
	if ((HasAuthority() && IsLocalPlayerController()) || !HasAuthority()) {
		if (!MenuWidgetBPClassRef) return;

		MenuWidgetRef = CreateWidget<UMenuWidget>(this, MenuWidgetBPClassRef);
		check(MenuWidgetRef);
		MenuWidgetRef->AddToViewport();

		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
}

void AMenuController::HostServer(const FString& ServerName, const int32& NumberOfPlayers)
{
	RequestHostServer(ServerName, NumberOfPlayers);
}

void AMenuController::RequestHostServer_Implementation(const FString& ServerName, const int32& NumberOfPlayers)
{
	AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
	
	if (IMenuInterface* GameModeInterface = Cast<IMenuInterface>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameModeInterface->HostServer(ServerName, NumberOfPlayers);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get game mode"));
	}
}

