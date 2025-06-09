// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Interactable.h"
#include "UserInterface/InteractionWidget.h"

// Sets default values for this component's properties
UAC_Interactable::UAC_Interactable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InteractionWidget = nullptr;
	InteractableName = "";
	ActionText = this->GetActionName();
	bCanInteract = true;
	InteractionSceneMeshReference = nullptr;
	OverrideInteractionDistance = 0;
	DetectMeshReference = false;

	// ...
}

void UAC_Interactable::ToggleInteractionWidget(bool Value)
{
	InteractionWidget->SetVisibility(Value,false);
}

void UAC_Interactable::SetActionText(FString NewText)
{
	ActionText = NewText;
	if (IsValid(InteractionWidget))
	{
		
		//Cast<UInteractionWidget>(InteractionWidget->GetWidget())->SetActionText(ActionText);

		
		if (auto Widget = InteractionWidget->GetWidget())
		{

			if (Widget->GetClass()->ImplementsInterface(UInteractionWidgetInterface::StaticClass()))
			{
				IInteractionWidgetInterface::Execute_UpdateInteractionActionText(InteractionWidget->GetWidget(), this->GetActionName());	
			}
		}
		
		
	}
}

FString UAC_Interactable::GetActionName()
{

	switch (ActionType) {
		case ActionTextType::USE:
			return TEXT("Use");
			break;

		case ActionTextType::OPEN:
			return TEXT("Open");
			break;

		case ActionTextType::PICKUP:
			return TEXT("Pickup");
			break;

		case ActionTextType::READ:
			return TEXT("Read");
			break;

		default:
			return ActionText;
		break;

	}
}

// Called when the game starts
void UAC_Interactable::BeginPlay()
{
	Super::BeginPlay();


	if (GetOwner())
	{
		InteractionWidget = GetOwner()->FindComponentByClass<UWidgetComponent>();
		if (InteractionWidget == nullptr) {
			InteractionWidget = NewObject<UWidgetComponent>(GetOwner(),TEXT("InteractPrompt"));
			InteractionWidget->SetupAttachment(this->GetOwner()->GetRootComponent());
		}

		FSoftClassPath InteractionWidgetBP(TEXT("/Game/Blueprints/Widgets/WBP_InteractionWidget.WBP_InteractionWidget_C"));

		TSubclassOf<UInteractionWidget> InteractionWidgetClass = InteractionWidgetBP.TryLoadClass<UInteractionWidget>();
		
		if (InteractionWidgetClass) {
			InteractionWidget->SetWidgetClass(InteractionWidgetClass);
		}
		
		InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
		InteractionWidget->SetDrawSize(FVector2D(500, 500));
	//	InteractionWidget->SetPivot({0.22f, 0.5f});
		InteractionWidget->SetDrawAtDesiredSize(true);
		//InteractionWidget->SetHiddenInGame(true, false);
		InteractionWidget->SetCollisionProfileName("NoCollision", true);
		//InteractionWidget->SetWorldLocation(this->GetComponentLocation());
		InteractionWidget->SetVisibility(false,false);

		
		if (!InteractionWidget->IsRegistered()) {
			InteractionWidget->RegisterComponent();
		}
		
		
		InteractionWidget->InitWidget();
		//InteractionWidget->UpdateWidget();
		
		
		
		SetActionText(GetActionName());

		if (!InteractionSceneMeshReference && DetectMeshReference)
		{
			InteractionSceneMeshReference = Cast<USceneComponent>(GetOwner()->FindComponentByTag(USceneComponent::StaticClass(),"InteractionSceneMeshReference"));
		}

		
	}
	



	
}


// Called every frame
void UAC_Interactable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

