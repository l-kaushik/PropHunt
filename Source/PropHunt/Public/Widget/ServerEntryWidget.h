// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerEntryWidget.generated.h"

class UTextBlock;
class UButton;
class AMenuController;

UCLASS()
class PROPHUNT_API UServerEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetServerNameText(const FString& InSessionName);
	void SetPingText(const FString& InPing);
	void SetStatusText(const FString& InStatus);
	void SetSessionResultIndex(int32 InServerResultIndex);
	
private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnJoinButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatusText;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	int32 SessionResultIndex;
	AMenuController* MenuController;
};
