// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "AC_Interactable.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AC_Interaction.generated.h"

USTRUCT()
struct FIteractionSelectionStruct
{
	GENERATED_BODY()

	UPROPERTY()
	UAC_Interactable* Interactable = nullptr;
	
	UPROPERTY()
	double Distance = 0;;
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTERACTIONGAME_API UAC_Interaction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_Interaction();

	UFUNCTION(BlueprintCallable)
	void SetupInteractionComponent(USceneComponent* const Source);

	void InteractionDetection(const FVector& InteractionSource);

	UFUNCTION(BlueprintCallable)
	void InteractAction();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float InteractionRadius;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	double InteractionDistance;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double InteractionCooldown;

	UPROPERTY()
	UAC_Interactable* Interaction;

	UPROPERTY()
	USceneComponent* InteractionDetectionSource;

	UFUNCTION(BlueprintCallable)
	void StopAndHideInteractions(const FString& Reason);
	
	UFUNCTION(BlueprintCallable)
	void ResumeAndShowInteractions(const FString& Reason);

	UFUNCTION(BlueprintCallable)
	bool IsInteractionEnabled() const { return bInteractionInitialized && InteractionLock.IsEmpty(); }

	UFUNCTION(BlueprintCallable)
	static FVector GetInteractionPosition(UAC_Interactable* InInteractable);


	UFUNCTION(BlueprintCallable)
	double GetInteractableInteractionDistance(UAC_Interactable* InInteractable);

	UFUNCTION(BlueprintCallable)
	UAC_Interactable* GetInteraction() const { return Interaction; }
	
protected:

	TArray<FString> InteractionLock; 
	
	UPROPERTY(VisibleInstanceOnly)
	bool bInteractionInitialized;
	
	double NextInteractionTime;
	
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};


