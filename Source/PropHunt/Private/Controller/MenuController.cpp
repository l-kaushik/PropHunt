// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MenuController.h"
#include "Widget/MenuWidget.h"
#include "GameModes/MenuGameMode.h"
#include "Subsystem/PropHuntSubsystem.h"

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
	// store subsystem

	if (HasAuthority())
	{
		PropHuntSubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UPropHuntSubsystem>();
	}

	if ((HasAuthority() && IsLocalPlayerController()) || !HasAuthority()) {
		if (!MenuWidgetBPClassRef) return;

		MenuWidgetRef = CreateWidget<UMenuWidget>(this, MenuWidgetBPClassRef);
		check(MenuWidgetRef);
		MenuWidgetRef->AddToViewport();

		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}

}

void AMenuController::ClientWantsToHost(const FName& SessionName, const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	ClientWantsToHostOnServer(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}

void AMenuController::ClientWantsToHostOnServer_Implementation(const FName& SessionName, const FString& LevelName, int32 NumPublicConnections, bool IsLANMatch)
{
	PropHuntSubsystem->OnCreateSessionCompleteEvent.AddUObject(this, &ThisClass::OnCreateSessionCompleted);
	PropHuntSubsystem->CreateSession(SessionName, LevelName, NumPublicConnections, IsLANMatch);
}

// callback functions for session management
void AMenuController::OnCreateSessionCompleted(bool Successful)
{
	if (Successful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Controller: session created successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu Controller: Failed to create session"));
	}
}
