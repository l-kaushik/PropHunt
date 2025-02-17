// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class PROPHUNT_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
	// Singleton accesser
	static UUIManager* Get();

	void InitializeWidgets();

	UPROPERTY()
	TSubclassOf<class UHostWidget> HostWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UJoinGameWidget> JoinGameWidgetBPClassRef;

	UPROPERTY()
	TSubclassOf<class UUIErrorBox> UIErrorBoxBPClassRef;

private:
	// Private constructor to enforce singleton
	UUIManager();
	
};
