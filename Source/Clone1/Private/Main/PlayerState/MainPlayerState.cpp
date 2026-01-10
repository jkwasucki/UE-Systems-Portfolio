// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/PlayerState/MainPlayerState.h"

#include "Main/PlayerState/AttributesComponent.h"
#include "Inventory/InventoryComponent.h"


AMainPlayerState::AMainPlayerState()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>("AttributesComponent");
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryComponent->SetEquipmentComponentLink(EquipmentComponent);
	EquipmentComponent->SetInventoryComponentLink(InventoryComponent);
	AttributesComponent->SetEquipmentComponentLink(EquipmentComponent);
	AttributesComponent->SetInventoryComponentLink(InventoryComponent);
}


