// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidgetInterface.h"
#include "InteractionWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INTERACTIONGAME_API UInteractionWidget : public UUserWidget, public IInteractionWidgetInterface
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	UTextBlock* InteractionActionText;

public:
	//void SetActionText(const FString& ActionText);
	virtual void UpdateInteractionActionText_Implementation(const FString& ActionText) override;
	
	
};
