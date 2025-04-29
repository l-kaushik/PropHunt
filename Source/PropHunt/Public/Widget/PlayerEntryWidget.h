// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEntryWidget.generated.h"

class UTextBlock;
class UHorizontalBox;
UCLASS()
class PROPHUNT_API UPlayerEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerNameText(const FString& InPlayerName);
	void SetPingText(const FString& InPing);
	void SetReadyStatusText(const FString& InReadyStatus);
	void SetReadyStatus(const bool IsReady);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyStatusText;
	
};
