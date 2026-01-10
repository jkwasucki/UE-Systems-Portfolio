// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractableComponent.h"
#include "GameFramework/Character.h"
#include "Main/PlayerController/MainPlayerController.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInteractableComponent::Interact_Implementation(ACharacter* Character)
{
	bIsInteractedWith = true;
	OnInteractDelegate.Broadcast(Character);
}

void UInteractableComponent::Highlight_Implementation(bool bState)
{
	OnHighlightDelegate.Broadcast(bState);
}


void UInteractableComponent::SetupComponent(AActor* inParent,USphereComponent* inCollider)
{
	if (inCollider && inParent)
	{
		Parent = inParent;
		Collider = inCollider;
		Collider->OnComponentBeginOverlap.AddDynamic(this, &UInteractableComponent::OnBeginOverlap);
		Collider->OnComponentEndOverlap.AddDynamic(this, &UInteractableComponent::OnEndOverlap);
		
		Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Collider->SetCollisionObjectType(ECC_WorldDynamic);
		Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
		Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Collider->SetGenerateOverlapEvents(true);
	}
}

FText UInteractableComponent::GetActionText_Implementation() 
{
	return ActionText;
}

FText UInteractableComponent::GetActionKeyString_Implementation() 
{
	return ActionKey;
}


void UInteractableComponent::OnBeginOverlap(   UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Character->GetController()))
		{
			if (PC)
			{
				if (PC->InteractionComponent)
				{
					
					PC->InteractionComponent->NearbyInteractables.Add(Parent);
					
				}
			}
		}
	}
}

void UInteractableComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Character->GetController()))
		{
			if (PC->InteractionComponent)
			{
				PC->InteractionComponent->NearbyInteractables.Remove(Parent);
			}
		}
	}
}
