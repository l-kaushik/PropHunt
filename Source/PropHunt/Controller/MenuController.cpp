// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuController.h"
#include "../Widget/MenuWidget.h"
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


