// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Structs/ScoreboardData.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchCardWidget.generated.h"

class UBorder;
class UButton;
class UTextBlock;
class UScoreboardMenuWidget;

struct FMatchData;

UCLASS()
class PROPHUNT_API UMatchCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void SetData(FMatchData InMatchData);

private:
	UFUNCTION()
	void OnMatchDetailClicked();
	void UpdateScoreboardToggleState();
	void SetDataInScoreboard();

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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MatchDetailLabel;

	UPROPERTY(meta = (BindWidget))
	UScoreboardMenuWidget* ScoreboardMenu;

	static const FLinearColor BorderTint;
	bool IsScoreboardMenuVisible;
	FScoreboardData ScoreboardData;
};
