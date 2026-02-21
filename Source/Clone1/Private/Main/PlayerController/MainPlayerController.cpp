// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerController/MainPlayerController.h"
#include "Actors/ArcadeMachine.h"
#include "EnhancedInputComponent.h"
#include "SaveSystem/GameSaveSubsystem.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "Main/PlayerState/MainPlayerState.h"


AMainPlayerController::AMainPlayerController()
{
	InputHandlerComponent = CreateDefaultSubobject<UInputHandlerComponent>("InputHandlerComponent");
	HUDComponent = CreateDefaultSubobject<UHUDComponent>("HUDComponent");
	HUDComponent->SetComponentTickEnabled(true);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		if (HUDComponent)
		{
			HUDComponent->Init(this);
		}
	}
	
	if (InputHandlerComponent)
	{
		InputHandlerComponent->OnInputSaveGameDelegate.AddDynamic(this, &AMainPlayerController::RequestSaveGame);
	}
	
	// INITIAL GAME LOAD
	if (HasAuthority())
	{
		if (UGameSaveSubsystem* GameSaveSubsystem = GetGameInstance()->GetSubsystem<UGameSaveSubsystem>())
		{
			GameSaveSubsystem->LoadGame(TEXT("DefaultSlot"));
		}
	}
	
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

void AMainPlayerController::BindInventoryDelegates()
{
	//Capture drop requests from InventoryComponent (PlayerState)
	AMainPlayerState* PS = GetPlayerState<AMainPlayerState>();
	if (PS && PS->InventoryComponent)
	{
		PS->InventoryComponent->OnRequestDropDelegate.AddDynamic(
			this,
			&AMainPlayerController::SpawnItemActor
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState or InventoryComponent not ready in BeginPlay"));
	}
}

void AMainPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BindInventoryDelegates();
}


void AMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BindInventoryDelegates();
}

UHUDComponent* AMainPlayerController::GetHUDComponent() const
{
	return FindComponentByClass<UHUDComponent>();
}

AMainCharacter* AMainPlayerController::GetMainCharacter() const
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetPawn());
	if (!MainCharacter) return nullptr;
	return MainCharacter;
}

void AMainPlayerController::RequestSaveGame()
{
	
	UGameSaveSubsystem* GameSaveSubsystem = GetGameInstance()->GetSubsystem<UGameSaveSubsystem>();
	if (!GameSaveSubsystem) return;
	
	GameSaveSubsystem->SaveGame(TEXT("DefaultSlot"));
}

FVector AMainPlayerController::GetCharacterLocation()
{
	return GetCharacter()->GetActorLocation();
}
