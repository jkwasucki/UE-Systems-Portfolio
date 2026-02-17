// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/GameSaveSubsystem.h"

#include "SaveSystem/SaveGameObject.h"
#include "Interfaces/SavableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Main/PlayerState/MainPlayerState.h"


void UGameSaveSubsystem::ApplyPendingLoad(APawn* Pawn)
{
	if (!bIsLoadPending) return;
	if (!Pawn)
	{
		UE_LOG(LogTemp,Warning, TEXT("Apply Load Failed: Pawn is null"));
		return;
	}
	AMainPlayerState* PS = Cast<AMainPlayerState>(Pawn->GetPlayerState());
	if (!PS) return;
	
	TArray<UActorComponent*> Components;
	Pawn->GetComponents(Components);
	
	
	for (UActorComponent* Component : Components)
	{
		if (Component->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
		{
			
			FName ID = ISavableInterface::Execute_GetSaveID(Component);
			if (PendingLoadData.PlayerData.ComponentData.Contains(ID))
			{
				const TArray<uint8> BinaryData = PendingLoadData.PlayerData.ComponentData[ID].Data;
				ISavableInterface::Execute_DeserializeFromBinary(Component,BinaryData);
			}
		}
	}
	
	TArray<UActorComponent*> PlayerStateComponents;
	PS->GetComponents(PlayerStateComponents);
	
	for (UActorComponent* Component : PlayerStateComponents)
	{
		if (Component->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
		{
			FName ID = ISavableInterface::Execute_GetSaveID(Component);
			if (PendingLoadData.PlayerData.ComponentData.Contains(ID))
			{
				const TArray<uint8> BinaryData = PendingLoadData.PlayerData.ComponentData[ID].Data;
				ISavableInterface::Execute_DeserializeFromBinary(Component,BinaryData);
			}
		}
	}
	
	bIsLoadPending = false;
	
	UE_LOG(LogTemp,Warning, TEXT("ApplyPendingLoad complete and applied to player."));
}


void UGameSaveSubsystem::LoadGame(FName SlotName)
{
	if (!UGameplayStatics::DoesSaveGameExist(ActiveSlot.ToString(),0))
	{
		UE_LOG(LogTemp,Warning,TEXT("LoadGame Failed: Slot does not exist (%s)"), *ActiveSlot.ToString())
		return;
	}
	
	USaveGame* LoadedObject  = UGameplayStatics::LoadGameFromSlot( ActiveSlot.ToString(),0);
	if (!LoadedObject)
	{
		UE_LOG(LogTemp,Warning, TEXT("LoadGame Failed: Could not load object"))
		return;
	}
	
	USaveGameObject* SaveObject = Cast<USaveGameObject>(LoadedObject);
	if (!SaveObject)
	{
		UE_LOG(LogTemp,Warning,TEXT("LoadGame Failed: Cast of USaveGame failed"))
		return;
	}
	
	PendingLoadData = SaveObject->SaveData;
	bIsLoadPending = true;
	
	UE_LOG(LogTemp,Warning,TEXT("LoadGame successful. Pending load."))
}

void UGameSaveSubsystem::SaveGame(FName SlotName)
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return;
	
	APawn* Pawn = PlayerController->GetPawn();
	if (!Pawn) return;
	
	AMainPlayerState* PS = Cast<AMainPlayerState>(Pawn->GetPlayerState());
	if (!PS) return;	
	
	FGameSaveData GameSaveData;
	
	
	// CHARACTER COMPONENTS
	TArray<UActorComponent*> CharacterComponents;
	Pawn->GetComponents(CharacterComponents);
	for (UActorComponent* Component : CharacterComponents)
	{
		if (Component->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
		{
			TArray<uint8> BinaryData;
			FName ID = ISavableInterface::Execute_GetSaveID(Component);
			ISavableInterface::Execute_SerializeToBinary(Component,BinaryData);
	
			FComponentBinaryData Wrapper;
			Wrapper.Data = BinaryData;

			GameSaveData.PlayerData.ComponentData.Add(ID, Wrapper);
		}
	}
	
	
	// STATE COMPONENTS
	TArray<UActorComponent*> PlayerStateComponents;
	PS->GetComponents(PlayerStateComponents);
	for (UActorComponent* Component : PlayerStateComponents)
	{
		if (Component->GetClass()->ImplementsInterface(USavableInterface::StaticClass()))
		{
			TArray<uint8> BinaryData;
			FName ID = ISavableInterface::Execute_GetSaveID(Component);
			ISavableInterface::Execute_SerializeToBinary(Component,BinaryData);
			
			FComponentBinaryData Wrapper;
			Wrapper.Data = BinaryData;
			GameSaveData.PlayerData.ComponentData.Add(ID, Wrapper);
		}
	}
	
	USaveGameObject* SaveObject = Cast<USaveGameObject>(UGameplayStatics::CreateSaveGameObject(USaveGameObject::StaticClass()));
	SaveObject->SaveData = GameSaveData;
	
	UGameplayStatics::SaveGameToSlot(SaveObject,ActiveSlot.ToString(),0);
}
