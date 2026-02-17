// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/Actions/PickupItemAction.h"
#include "Engine/Engine.h"
#include "InteractionSystem/Instances/InteractionInstance.h"
#include "Main/Character/Derived/MainCharacter.h"
#include "Main/PlayerState/MainPlayerState.h"

void UPickupItemAction::Execute(UInteractionInstance* Instance)
{
	if (!IsValid(Instance) || !Instance->Instigator.IsValid()) return;
	
	AActor* Target = Instance->Target.Get();
	AActor* Instigator = Instance->Instigator.Get();
	
	AItem* Item = Cast<AItem>(Target);
	ACharacter* Character = Cast<AMainCharacter>(Instigator);
	
	if (!Character) return;
	
	AMainPlayerState* PS = Cast<AMainPlayerState>(Character->GetPlayerState());
	
	if (PS && Item)
	{
		if (PS->InventoryComponent)
		{
			if (PS->InventoryComponent->AddItem(Item->ItemStack.ItemID, Item->ItemStack.Amount))
			{
				Target->Destroy();
			}
		}
	}
}
