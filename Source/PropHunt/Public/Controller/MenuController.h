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

	void CreateHostWidget();
	void CreateJoinWidget();

protected:
	UFUNCTION(Server, Reliable)
	void ClientWantsToHostOnServer(const FName& SessionName,const FString& LevelName, int32 NumPublicConnections, bool IsLANMatc);

protected:
	// callback functions for session management
	void OnCreateSessionCompleted(bool Successful);

private:
	template<typename T>
	T* CreateAndAddWidget(UClass* WidgetBPClassRef)
	{
		if (!WidgetBPClassRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("WidgetBPClassRef is invalid"));
			return nullptr;
		}

		T* WidgetRef = CreateWidget<T>(this, WidgetBPClassRef);

		if (!WidgetRef)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create widget, check template argument type and WidgetBPClassRef"));
			return nullptr;
		}

		WidgetRef->AddToViewport();

		return WidgetRef;
	}

	template<typename T, typename U>
	T* CreateSubWidgetAndHideParent(UClass* WidgetBPClassRef, U* ParentWidget)
	{
		T* WidgetRef = CreateAndAddWidget<T>(WidgetBPClassRef);

		if (WidgetRef)
		{
			WidgetRef->SetParentWidget(ParentWidget);
			ParentWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		return WidgetRef;
	}

private:
	TSubclassOf<UMenuWidget> MenuWidgetBPClassRef;
	UMenuWidget* MenuWidgetRef;

	TSubclassOf<class UHostWidget> HostWidgetBPClassRef;
	TSubclassOf<class UJoinGameWidget> JoinGameWidgetBPClassRef;

	UPropHuntSubsystem* PropHuntSubsystem;
};
