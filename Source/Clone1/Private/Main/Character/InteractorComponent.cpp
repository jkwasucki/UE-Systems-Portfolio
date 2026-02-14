// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/InteractorComponent.h"
#include "Interfaces/InteractableInterface.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Main/Character/Derived/MainCharacter.h"


UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(true);
}

void UInteractorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	QueryEntityUnderCursor();
	QueryInteractableUnderCursor();
	HandleHighlight();
}

void UInteractorComponent::Init(AMainCharacter* InOwner)
{
	if (!IsValid(InOwner)) return;
	Owner = InOwner;
}


void UInteractorComponent::Interact()
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
	
	if (IsValid(CurrentLoadedDefinition))
	{
		ExecuteInteraction(CurrentLoadedDefinition, Best);
	}
}
void UInteractorComponent::ExecuteInteraction(UInteractionDefinition* InteractionDefinition, AActor* Target)
{
	if (UInteractionInstance* NewInteraction = NewObject<UInteractionInstance>(this))
	{
		NewInteraction->Init(GetOwner(),Target,InteractionDefinition);
		InstancedInteractions.Add(NewInteraction);
		
		OnInteractableFocusEndDelegate.Broadcast();
	}
}

void UInteractorComponent::QueryInteractableUnderCursor()
{
	AActor* Best = GetBestInteractable();
	if (Best == CurrentFocusedInteractable.Get())
		return;
	
	if (CurrentFocusedInteractable.IsValid())
	{
		OnInteractableFocusEndDelegate.Broadcast();
	}

	PreviousFocusedInteractable = CurrentFocusedInteractable;
	CurrentFocusedInteractable = Best;

	// Clear current definition
	CurrentLoadedDefinition = nullptr;

	// Handle new focus
	if (CurrentFocusedInteractable.IsValid())
	{
		AActor* Actor = CurrentFocusedInteractable.Get();

		if (Actor->Implements<UInteractableInterface>())
		{
			TSoftObjectPtr<UInteractionDefinition> SoftDef =
				IInteractableInterface::Execute_GetInteractionDefinition(Actor);

			if (SoftDef.IsValid())
			{
				CurrentLoadedDefinition = SoftDef.LoadSynchronous();
			}
		}

		OnInteractableFocused.Broadcast(CurrentLoadedDefinition, Actor);
	}
}



void UInteractorComponent::HandleHighlight()
{
	if (CurrentFocusedInteractable == PreviousFocusedInteractable)
		return;
	
	if (PreviousFocusedInteractable.IsValid())
		IInteractableInterface::Execute_Highlight(PreviousFocusedInteractable.Get(),false);
	if (CurrentFocusedInteractable.IsValid())
		IInteractableInterface::Execute_Highlight(CurrentFocusedInteractable.Get(),true);
}


AActor* UInteractorComponent::GetBestInteractable()
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

void UInteractorComponent::AddInteractable(AActor* Actor)
{
	if (!IsValid(Actor)) return;
	NearbyInteractables.AddUnique(Actor);
}

void UInteractorComponent::RemoveInteractable(AActor* Actor)
{
	NearbyInteractables.Remove(Actor);
}



void UInteractorComponent::ProcessResultUnderCursor(bool bHit,  const FHitResult& Hit)
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

void UInteractorComponent::QueryEntityUnderCursor()
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

void UInteractorComponent::GetEntityData(AActor* Actor)
{
	if (Actor->Implements<UDebugInfoProviderInterface>())
	{
		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Actor);
		FEntityGameplayDebugSnapshot Snapshot = IDebugInfoProviderInterface::Execute_GetDebugInfo(Actor);
		Owner->GetDebugComponent()->OnEntityDebugSnapshotDelegate.Broadcast(BaseCharacter,Snapshot);
	}
}
