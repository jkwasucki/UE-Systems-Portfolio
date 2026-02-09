// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character/EnemyAIComponent.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/ResourceInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API AEnemyCharacter : public ABaseCharacter, public IResourceInterface, public IDamageableInterface, public ICharacterEffectReceiverInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UEnemyAIComponent* EnemyAIComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* AbilityProjectileSpawnPoint;
	
	
	UFUNCTION()
	void ApplyAbilityVisuals(UAbilityData* Ability,  FGuid InstanceID,  FAbilityTargetData& Targets);
	UFUNCTION()
	void CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier);
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAbilityData* AvailableAbility;
	
	
	
	AEnemyCharacter();
	
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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() override;
	
};
