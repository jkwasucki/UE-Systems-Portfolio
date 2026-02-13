// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "GameFramework/PlayerState.h"
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
public:
	AMainPlayerState();
	
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UEquipmentComponent* EquipmentComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UQuestComponent* QuestSystemComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAbilityData*> AvailableAbilities;			// WHAT IS AVAILABLE TO USE
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FFAbilitySlot> AbilitySlots;					// ABILITY PER KEY SLOT
	
	UFUNCTION()
	TArray<UAbilityData*> GetAbilities() const;
	UFUNCTION()
	UAbilityData* GetAbilityBySlot(FGameplayTag Tag);

};
