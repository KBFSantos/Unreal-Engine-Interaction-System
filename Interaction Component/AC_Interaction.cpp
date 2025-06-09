// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_Interaction.h"

// Sets default values for this component's properties
UAC_Interaction::UAC_Interaction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	InteractionRadius = 250;
	InteractionDistance = 200;
	NextInteractionTime = 0;
	InteractionDetectionSource = nullptr;
	Interaction = nullptr;
	bInteractionInitialized = false;
	InteractionDetectionSource = nullptr;
}

void UAC_Interaction::InteractionDetection(const FVector& InteractionSource)
{
	if (IsInteractionEnabled())
	{
		FIteractionSelectionStruct SelectedInteractable = {};
		
		if (IsValid(Interaction))
		{
			Interaction->ToggleInteractionWidget(false);
		}

		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Vehicle));

		TArray<AActor*> ignoreActors;
		ignoreActors.Init(GetOwner(), 1);

		// Array of actors that are inside the radius of the sphere
		TArray<AActor*> outActors;
	

		//DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), InteractionRadius, 12, FColor::Red, false);
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), InteractionRadius, traceObjectTypes, AActor::StaticClass(), ignoreActors, outActors);
		for (AActor* overlappedActor : outActors)
		{

			if (overlappedActor->Implements<UInteractableInterface>())
			{
				if (UAC_Interactable* InteractableComp = overlappedActor->FindComponentByClass<UAC_Interactable>())
				{
					
				
					FVector Pos = GetInteractionPosition(InteractableComp);
					

				

					FCollisionQueryParams TraceParams;
					TraceParams.AddIgnoredActor(GetOwner());
					FHitResult Hit;
					if (GetWorld()->LineTraceSingleByChannel(Hit, InteractionSource, Pos, ECollisionChannel::ECC_Visibility, TraceParams))
					{
							if (Hit.GetActor() == overlappedActor)
							{
							//	DrawDebugLine(GetWorld(), InteractionSource, Pos, FColor::Red, false);
						//		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 10.0f, FColor::Green, false);
								float InteractableDistance = FVector::Distance(InteractionSource, Hit.ImpactPoint);
								if (InteractableDistance <= GetInteractableInteractionDistance(InteractableComp))
								{
									
									auto GetComparisonDistance = [&](const UAC_Interactable* InteractableComparable) ->float { return (InteractableComparable->CompareDistanceByWidget ? FVector::Distance(InteractableComparable->GetInteractionWidget()->GetComponentLocation(), InteractionSource) : InteractableDistance); };
									
									if (SelectedInteractable.Interactable != nullptr)
									{
										float CurrentDistance = GetComparisonDistance(InteractableComp);
										if (CurrentDistance < SelectedInteractable.Distance)
										{
											SelectedInteractable.Interactable = InteractableComp;
											SelectedInteractable.Distance = CurrentDistance;
										}
									}
									else
									{
										SelectedInteractable.Interactable = InteractableComp;
										SelectedInteractable.Distance = GetComparisonDistance(SelectedInteractable.Interactable);
									}
									

								}

							}
						
					}
				}
			}
		}

		if (SelectedInteractable.Interactable != nullptr)
		{


			if (SelectedInteractable.Interactable->bCanInteract)
			{
				Interaction = SelectedInteractable.Interactable;
				Interaction->ToggleInteractionWidget(true);
			}
			SelectedInteractable = {};

		}
		else
		{
			Interaction = nullptr;
		}

	}
}

void UAC_Interaction::SetupInteractionComponent(USceneComponent* const Source)
{
	InteractionDetectionSource = Source;
	if (IsValid(InteractionDetectionSource)) {
		this->SetComponentTickEnabled(true);
		bInteractionInitialized = true;
	}
}

void UAC_Interaction::InteractAction()
{
	if (IsInteractionEnabled())
	{
		double ElapsedSeconds = GetWorld()->GetTimeSeconds();
		if (ElapsedSeconds > NextInteractionTime)  // Interaction Cooldown Check
		{
			if (!IsValid(Interaction)) 
			{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
            	if (GEngine) 
            	{
            		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Interaction not found");
            	}
#endif
            	return;
			}

			if (!Interaction->bCanInteract)
			{
				return;
			}
			
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s Interacted"), *Interaction->InteractableName));
			}
#endif

			if (Interaction->GetOwner()->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			{
				IInteractableInterface::Execute_Interact(Interaction->GetOwner(), Cast<ACharacter>(GetOwner()));
			}
			else
			{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s doesn't has a Interaction Interface"), *Interaction->InteractableName));
				}
#endif
				}
			NextInteractionTime = ElapsedSeconds + InteractionCooldown; // After Interaction Start Cooldown
		}
	}
}


void UAC_Interaction::StopAndHideInteractions(const FString& Reason)
{
	InteractionLock.AddUnique(Reason);
	if (IsValid(Interaction))
	{
		Interaction->ToggleInteractionWidget(false);
	}
	
}

void UAC_Interaction::ResumeAndShowInteractions(const FString& Reason)
{
	InteractionLock.RemoveSingle(Reason);
}

FVector UAC_Interaction::GetInteractionPosition(UAC_Interactable* InInteractable)
{

	if (!InInteractable)
		return{};


	if (InInteractable->InteractionSceneMeshReference)
		return InInteractable->InteractionSceneMeshReference->GetComponentLocation();

	
	return InInteractable->GetOwner()->GetActorLocation();
}

double UAC_Interaction::GetInteractableInteractionDistance(UAC_Interactable* InInteractable)
{
	return InInteractable->OverrideInteractionDistance > 0 ? InInteractable->OverrideInteractionDistance : InteractionDistance;
}

// Called when the game starts
void UAC_Interaction::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UAC_Interaction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...

	if (GetWorld() != nullptr) {
		InteractionDetection(InteractionDetectionSource->GetComponentLocation());
	}
}

