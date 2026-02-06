// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character/AbilitySystemComponent.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Main/Character/EffectsComponent.h"
#include "Types/ItemTypes.h"
#include "GameplayTagContainer.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/ResourceInterface.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "MainCharacter.generated.h"


UCLASS()
class CLONE1_API AMainCharacter : public ABaseCharacter, public IResourceInterface, public ICharacterEffectReceiverInterface, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UFUNCTION()
	void ApplyAbilityVisuals(UAbilityData* Ability, FGuid InstanceID, FAbilityTargetData& Targets);
	UFUNCTION()
	void CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier);
public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* AbilityProjectileSpawnPoint;
	
	// IResource Interface
	 virtual float GetEnergy_Implementation() const override;
	virtual void ModifyEnergy_Implementation(float Delta) override;
	virtual float GetHealth_Implementation() const override;
	virtual void ModifyHealth_Implementation(float Delta) override;
	
	
	// IDamageable Interface
	virtual void TakeDamage_Implementation(float Value) override;
	
	// CharacterEffectReciever Interface
	virtual void ApplyEffect_Implementation(AActor* EffectOrigin,FCharacterEffect& Effect, FGuid SourceInstanceID) override;	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() override;
	
	virtual void OnRespondToHealthChange(float Delta) override;
	

	FOnStateRequest OnStateRequestDelegate;
	
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	USkeletalMeshComponent* HeadMesh = nullptr;
	UPROPERTY(VisibleAnywhere ,Category = "Equipment")
	USkeletalMeshComponent* ChestMesh = nullptr;
	UPROPERTY(VisibleAnywhere,Category = "Equipment")
	USkeletalMeshComponent* LegsMesh = nullptr;
	UPROPERTY(VisibleAnywhere,Category = "Equipment")
	USkeletalMeshComponent* HandsMesh = nullptr;
	UPROPERTY(VisibleAnywhere,Category = "Equipment")
	USkeletalMeshComponent* FeetMesh = nullptr;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void TryUseAbility(FGameplayTag SlotTag);
	UFUNCTION()
	void SetSkeletalDefaults(USkeletalMeshComponent* SkeletalMesh);
	UFUNCTION()
	void  EquipMesh(USkeletalMesh* MeshToEquip, EEquipmentType Type);
	UFUNCTION()
	void UnEquipMesh(EEquipmentType Type);
	UFUNCTION()
	void ApplyAttributes();
	UFUNCTION()
	void ResolveAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event);
	
	UFUNCTION()
	UAbilitySystemComponent* GetAbilitySystem();
	

	UFUNCTION()
	void AbilityCastFailDebugSnapshot( UAbilityData* Ability, EAbilityFailureReason Reason);
	UFUNCTION()
	void AbilityCastDebugSnapshot(UAbilityData* Ability,  FGuid InstanceID, FAbilityTargetData& Targets );


};

