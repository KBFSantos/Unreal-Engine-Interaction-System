// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionWidgetInterface.h"
#include "Components/WidgetComponent.h"
#include "Components/ActorComponent.h"
#include "AC_Interactable.generated.h"


UENUM(BlueprintType)
enum class ActionTextType : uint8 {
	USE = 0 UMETA(DisplayName = "Use"),
	OPEN = 1  UMETA(DisplayName = "Open"),
	PICKUP = 2     UMETA(DisplayName = "Pickup"),
	READ = 3	UMETA(DisplayName = "Read"),
	CUSTOM = 4 UMETA(DisplayName = "Custom Action"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONGAME_API UAC_Interactable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Interactable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
	FString InteractableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
	ActionTextType ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
	double OverrideInteractionDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings")
	bool bCanInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Settings", meta = (EditCondition = "ActionType == ActionTextType::CUSTOM"))
	FString ActionText;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Find the component with tag 'InteractionSceneMeshReference' and set to Reference Mesh") ,BlueprintReadWrite, Category = "Interaction Settings")
	bool DetectMeshReference;

	UPROPERTY(EditAnywhere, meta = (ToolTip = "Comparate distance by widget position with other interactables") ,BlueprintReadWrite, Category = "Interaction Settings")
	bool CompareDistanceByWidget;
	
	// Optional: for movable Objects
	UPROPERTY()
	USceneComponent* InteractionSceneMeshReference;

	UFUNCTION(BlueprintCallable)
	UWidgetComponent* GetInteractionWidget() const { return InteractionWidget; };

	UFUNCTION(BlueprintCallable)
	void ToggleInteractionWidget(bool Value);
	
	UFUNCTION(BlueprintCallable)
	void SetActionText(FString NewText);

	UFUNCTION(BlueprintCallable)
	FString GetActionName();


protected:

	UPROPERTY()
	UWidgetComponent* InteractionWidget;
	

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
