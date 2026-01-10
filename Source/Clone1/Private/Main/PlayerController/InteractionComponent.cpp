// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/InteractionComponent.h"

#include "Interfaces/Interactable.h"
#include "Components/InteractableComponent.h"
#include "Main/PlayerController/MainPlayerController.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);
	
	PC = Cast<AMainPlayerController>(GetOwner());
	PC->InputHandlerComponent->OnInteractDelegate.AddDynamic(this,&UInteractionComponent::Interact);
}

void UInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	HandleHighlight();
	
}


void UInteractionComponent::Interact()
{
	if (NearbyInteractables.IsEmpty()) return;
	IInteractable::Execute_Interact(GetBestInteractable(), PC->GetCharacter());
}

void UInteractionComponent::HandleHighlight()
{
	if (NearbyInteractables.IsEmpty())
	{
		if (FocusedActorComponent != nullptr)
			IInteractable::Execute_Highlight(FocusedActorComponent,false);
		
		FocusedActorComponent = nullptr;
		return;
	}
		
	UInteractableComponent* Best = GetBestInteractable();
	if (Best == nullptr) return;
	if (Best == FocusedActorComponent) return;
	
	if (FocusedActorComponent != nullptr)
		IInteractable::Execute_Highlight(FocusedActorComponent,false);
		
	FocusedActorComponent = Best;
	
	
	IInteractable::Execute_Highlight(FocusedActorComponent,true);
}

UInteractableComponent* UInteractionComponent::GetBestInteractable()
{
	if (!IsValid(PC)) return nullptr;

	APawn* Pawn = PC->GetPawn();
	if (!IsValid(Pawn)) return nullptr;

	UInteractableComponent* Best = nullptr;
	float BestDist = FLT_MAX;

	for (AActor* Actor : NearbyInteractables)
	{
		UInteractableComponent* InteractableComponent = Actor->GetComponentByClass<UInteractableComponent>();
		
		if (!IsValid(Actor)) continue;
		if (!InteractableComponent && !InteractableComponent->Implements<UInteractable>()) continue;
		
		float Dist = FVector::DistSquared(
			Actor->GetActorLocation(),
			Pawn->GetActorLocation()
		);

		if (Dist < BestDist)
		{
			BestDist = Dist;
			Best = InteractableComponent;
		}
	}

	return Best;
}


