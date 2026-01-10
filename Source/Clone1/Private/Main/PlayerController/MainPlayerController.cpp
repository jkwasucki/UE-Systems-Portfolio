// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/MainPlayerController.h"

#include "Actors/ArcadeMachine.h"
#include "PacMan/PacmanGame.h"
#include "EnhancedInputComponent.h"
#include "Main/PlayerState/MainPlayerState.h"


AMainPlayerController::AMainPlayerController()
{
	InputHandlerComponent = CreateDefaultSubobject<UInputHandlerComponent>("InputHandlerComponent");
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	HUDComponent = CreateDefaultSubobject<UHUDComponent>("HUDComponent");
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
	//Capture drop requests from InventoryComponent (PlayerState)
	GetPlayerState<AMainPlayerState>()->InventoryComponent->OnRequestDropDelegate.AddDynamic(this, &AMainPlayerController::SpawnItemActor);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EI =
		CastChecked<UEnhancedInputComponent>(InputComponent);

	InputHandlerComponent->SetupInput(EI);
}
void AMainPlayerController::SpawnItemActor(FItemStack ItemStack)
{
	const FItemData* ItemData = ItemsDataTable->FindRow<FItemData>(ItemStack.ItemID,TEXT(""));
	if (ItemData && ItemData->ItemBaseData.ItemClass)
	{
		FVector SL = GetDropLocation();
		SL.Z += 20.f;
		
		const FRotator SpawnRotation = FRotator(FRotator::ZeroRotator);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = 	ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AItem* SpawnedItem = 
				GetWorld()->SpawnActor<AItem>(
					ItemData->ItemBaseData.ItemClass,
					SL,
					SpawnRotation,
					SpawnParams
					);
		if (SpawnedItem)
		{
			SpawnedItem->InitItem(ItemStack);
			SpawnedItem->EnablePhysics();
		}
	}
}

FVector AMainPlayerController::GetDropLocation()
{
	APawn* P = GetPawn();
	if (P)
	{
		const FVector Forward = P->GetActorForwardVector();
		return P->GetActorLocation() + Forward * 150.f;
	}
	else
	{
		return FVector::ZeroVector;	
	}
}

void AMainPlayerController::StartPacmanGame(AArcadeMachine* ArcadeMachine)
{
	check(ArcadeMachine);
	
	SetViewTargetWithBlend(
	ArcadeMachine,
	0.6f,
	VTBlend_EaseInOut,
	2.0f
);

	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	
	ArcadeMachine->OnQuitDelegate.RemoveAll(this);
	ArcadeMachine->OnQuitDelegate.AddDynamic(this,&AMainPlayerController::StopArcade);
	
	ArcadeMachine->SetupArcade(InputHandlerComponent);
	
}

void AMainPlayerController::StopArcade()
{
	SetViewTargetWithBlend(
	GetCharacter(),
	0.6f,
	VTBlend_EaseInOut,
	2.0f
	);
	
	SetIgnoreLookInput(false);
	SetIgnoreMoveInput(false);
}
