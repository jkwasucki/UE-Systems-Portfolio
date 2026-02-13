// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/InteractionComponent.h"
#include "Interfaces/InteractableInterface.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Main/Character/Derived/MainCharacter.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);
}

void UInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	QueryEntityUnderCursor();
	QueryInteractableUnderCursor();
	HandleHighlight();
}

void UInteractionComponent::Init(AMainCharacter* InOwner)
{
	if (!IsValid(InOwner)) return;
	Owner = InOwner;
}


void UInteractionComponent::Interact()
{
	if (!Owner.IsValid())
		return;

	AActor* Best = GetBestInteractable();
	if (!IsValid(Best))
		return;
	if (!Best->Implements<UInteractableInterface>())
		return;
	AMainCharacter* OwnerCharacter = Owner.Get();
	if (!OwnerCharacter)
		return;
	
	IInteractableInterface::Execute_Interact(Best, OwnerCharacter);
}


void UInteractionComponent::QueryInteractableUnderCursor()
{
	AActor* Best = GetBestInteractable();

	if (Best == nullptr)
	{
		OnInteractableFocusEndDelegate.Broadcast();
		return;	
	}
	
	

	if (Best == CurrentFocusedInteractable)
		return;
	

	PreviousFocusedInteractable = CurrentFocusedInteractable;
	CurrentFocusedInteractable = Best;

	if (CurrentFocusedInteractable.IsValid() && PreviousFocusedInteractable.IsValid())
	{
		if (CurrentFocusedInteractable.Get() != PreviousFocusedInteractable.Get())
		{
			OnInteractableFocused.Broadcast(CurrentFocusedInteractable.Get());
		}
	}
}


void UInteractionComponent::HandleHighlight()
{
	
	if (CurrentFocusedInteractable == nullptr) return;

	if (PreviousFocusedInteractable == CurrentFocusedInteractable)
		IInteractableInterface::Execute_Highlight(PreviousFocusedInteractable.Get(),false);
	
	if (NearbyInteractables.IsEmpty())
	{
		if (CurrentFocusedInteractable != nullptr)
		{
			if (CurrentFocusedInteractable.IsValid())
				IInteractableInterface::Execute_Highlight(CurrentFocusedInteractable.Get(),false);
		}
		
		CurrentFocusedInteractable = nullptr;
		return;
	}
	if (PreviousFocusedInteractable.IsValid())
		IInteractableInterface::Execute_Highlight(PreviousFocusedInteractable.Get(),false);
	if (CurrentFocusedInteractable.IsValid())
		IInteractableInterface::Execute_Highlight(CurrentFocusedInteractable.Get(),true);
	
}


AActor* UInteractionComponent::GetBestInteractable()
{
	if (!Owner.IsValid()) return nullptr;
	AActor* Best = nullptr;
	float BestDist = FLT_MAX;

	for (TWeakObjectPtr<AActor> Actor : NearbyInteractables)
	{
		
		if (!Actor.IsValid()) continue;
		if (!Actor->Implements<UInteractableInterface>()) continue;
		
		float Dist = FVector::DistSquared(
			Actor->GetActorLocation(),
			Owner->GetActorLocation()
		);

		if (Dist < BestDist)
		{
			BestDist = Dist;
			Best = Actor.Get();
		}
	}
	return Best;
}



void UInteractionComponent::ProcessResultUnderCursor(bool bHit,  const FHitResult& Hit)
{
	bool bHitPawn = false;
	APawn* Pawn = nullptr;
	if (bHit)
	{
		Pawn = Cast<APawn>(Hit.GetActor());
		if (Pawn)
			bHitPawn = true;
	}
	
	if (!bHitPawn)
	{
		if (EntityUnderCursor.IsValid())
		{
			EntityUnderCursor = nullptr;
			OnNoEntityUnderCursorDelegate.Broadcast();
		}
		return;
	}
	
	if (ABaseCharacter* Char = Cast<ABaseCharacter>(Pawn))
	{
		if (Char != EntityUnderCursor)
			OnEntityUnderCursorDelegate.Broadcast(Hit.GetActor());
		
		EntityUnderCursor = Char;
		return;
	}

	if (EntityUnderCursor != nullptr)
	{
		EntityUnderCursor = nullptr;
		OnNoEntityUnderCursorDelegate.Broadcast();
	}
}

void UInteractionComponent::QueryEntityUnderCursor()
{
	if (!Owner.IsValid()) return;
	AMainCharacter* Character = Owner.Get();
	if (!Character) return;
	AMainPlayerController* PC = Cast<AMainPlayerController>(Character->GetController());
	if (!PC) return;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	
	int32 SizeX, SizeY;
	PC->GetViewportSize(SizeX, SizeY);

	const FVector2D ScreenCenter(
		SizeX * 0.5f,
		SizeY * 0.5f
	);
	
	FHitResult Hit;
	const bool bHit = PC->GetHitResultAtScreenPosition(
	   ScreenCenter,
	   ECC_PhysicsBody,
	   QueryParams,
	   Hit
   );

	ProcessResultUnderCursor(
		bHit,
		Hit
	);
}

void UInteractionComponent::GetEntityData(AActor* Actor)
{
	if (Actor->Implements<UDebugInfoProviderInterface>())
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Actor);
		FEntityGameplayDebugSnapshot Snapshot = IDebugInfoProviderInterface::Execute_GetDebugInfo(Actor);
		Owner->GetDebugComponent()->OnEntityDebugSnapshotDelegate.Broadcast(BaseCharacter,Snapshot);
	}
}
