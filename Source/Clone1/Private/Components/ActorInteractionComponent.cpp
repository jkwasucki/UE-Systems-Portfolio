// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActorInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "Main/PlayerController/MainPlayerController.h"

// Sets default values for this component's properties
UActorInteractionComponent::UActorInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




void UActorInteractionComponent::SetupComponent(AActor* inParent,USphereComponent* inCollider)
{
	if (inCollider && inParent)
	{
		Parent = inParent;
		Collider = inCollider;
		Collider->OnComponentBeginOverlap.AddDynamic(this, &UActorInteractionComponent::OnBeginOverlap);
		Collider->OnComponentEndOverlap.AddDynamic(this, &UActorInteractionComponent::OnEndOverlap);
		
		Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Collider->SetCollisionObjectType(ECC_WorldDynamic);
		Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
		Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Collider->SetGenerateOverlapEvents(true);
	}
}



void UActorInteractionComponent::OnBeginOverlap(   UPrimitiveComponent* OverlappedComp,
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
				AMainCharacter* MC = PC->GetMainCharacter();
				if (MC)
				{
					MC->GetInteractionComponent()->NearbyInteractables.Add(Parent);
				}
			}
		}
	}
}

void UActorInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Character->GetController()))
		{
			AMainCharacter* MC = PC->GetMainCharacter();
			if (MC)
			{
				MC->GetInteractionComponent()->NearbyInteractables.Remove(Parent);
			}
		}
	}
}
