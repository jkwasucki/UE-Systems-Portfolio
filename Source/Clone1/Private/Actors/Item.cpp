// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = SceneRoot;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SceneRoot);
	//For physics
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->bDisallowNanite = true;
	
	
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetSphereRadius(50.f);
	Collider->SetupAttachment(SceneRoot);


	InteractableComponent = CreateDefaultSubobject<UActorInteractionComponent>("InteractionComponent");
}
void AItem::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Mesh->SetGenerateOverlapEvents(false);
	
	if (!InteractableComponent) return;
	InteractableComponent->SetupComponent(this,Collider);
	InteractableComponent->OnHighlightDelegate.AddDynamic(this, &AItem::AItem::OnHighlight);
	
}
TSoftObjectPtr<UInteractionDefinition> AItem::GetInteractionDefinition_Implementation()
{
	return InteractableComponent->InteractionDefinition;
}

void AItem::OnHighlight(bool bState)
{
	Mesh->SetOverlayMaterial(bState ? M_Outline : nullptr);
}


void AItem::InitItem(const FItemStack& InStack)
{
	ItemStack.ItemID = InStack.ItemID;
	ItemStack.Amount = InStack.Amount;
}



void AItem::EnablePhysics()
{
	if (!Mesh)return;
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	Mesh->WakeAllRigidBodies();


	GetWorld()->GetTimerManager().SetTimer(
		PhysicsSettleTimer,
		this,
		&AItem::DisablePhysics,
		0.75f,   
		false
	);
}
void AItem::DisablePhysics()
{
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
void AItem::Highlight_Implementation(bool bState)
{
	InteractableComponent->OnHighlightDelegate.Broadcast(bState);
}

bool AItem::IsInteractedWith_Implementation()
{
	return InteractableComponent->bIsInteractedWith;
}



