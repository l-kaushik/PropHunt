// Fill out your copyright notice in the Description page of Project Settings.


/*
* TODO: Implement a common parent class for both PlayerEntryWidget and GameStatsEntryWidget
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerEntryWidget.generated.h"

class UImage;
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

	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Settings")
	bool bBackgroundDisabled;

private:
	UPROPERTY(meta = (BindWidget))
	UImage* Background;

	static const FLinearColor Backgroundcolor;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PingText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyStatusText;	
};
