// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterface/InteractionWidget.h"

#include "Components/TextBlock.h"

void UInteractionWidget::UpdateInteractionActionText_Implementation(const FString& ActionText)
{
	IInteractionWidgetInterface::UpdateInteractionActionText_Implementation(ActionText);

	InteractionActionText->SetText(FText::FromString(ActionText));
}
