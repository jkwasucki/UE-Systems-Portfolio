// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "GameFramework/PlayerState.h"
#include "Main/Character/AbilitySystemComponent.h"
#include "QuestSystem/QuestComponent.h"
#include "Structs/FAbilitySlot.h"
#include "MainPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAbilityData*> AvailableAbilities;			// WHAT IS AVAILABLE TO USE
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FFAbilitySlot> AbilitySlots;					// ABILITY PER KEY SLOT
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Components")
	UEquipmentComponent* EquipmentComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Components")
	UQuestComponent* QuestSystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UAbilitySystemComponent* AbilitySystemComponent;

protected:
	virtual void BeginPlay() override;
public:
	AMainPlayerState();
	UFUNCTION()
	TArray<UAbilityData*> GetAbilities() const;
	UFUNCTION()
	UAbilityData* GetAbilityBySlot(FGameplayTag SlotTag);
};
