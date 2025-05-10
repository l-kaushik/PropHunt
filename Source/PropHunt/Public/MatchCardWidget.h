// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchCardWidget.generated.h"

class UBorder;
class UButton;
class UTextBlock;

UCLASS()
class PROPHUNT_API UMatchCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	UFUNCTION()
	void OnMatchDetailClicked();

private:
	UPROPERTY(meta = (BindWidget))
	UBorder* Border;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchTime;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostName;

	UPROPERTY(meta = (BindWidget))
	UButton* MatchDetail;

	static const FLinearColor BorderTint;
};
