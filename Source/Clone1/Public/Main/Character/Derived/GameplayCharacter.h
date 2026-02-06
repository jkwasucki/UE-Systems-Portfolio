// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/ResourceInterface.h"
#include "GameplayCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API AGameplayCharacter : public ABaseCharacter, public IResourceInterface, public IDamageableInterface, public ICharacterEffectReceiverInterface
{
	GENERATED_BODY()
public:
	AGameplayCharacter();
	
	virtual void BeginPlay() override;
	
	// Resource Interface
	virtual float GetHealth_Implementation() const override;
	virtual float GetEnergy_Implementation() const override;
	virtual void ModifyEnergy_Implementation(float Delta) override;
	virtual void ModifyHealth_Implementation(float Delta) override;
	
	// Damageable Interface
	virtual void TakeDamage_Implementation(float Value)  override;
	// Character Effect Interface
	virtual void ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effects, FGuid SourceInstanceID) override;	
	
	
	virtual void OnRespondToHealthChange(float Delta) override;
};
