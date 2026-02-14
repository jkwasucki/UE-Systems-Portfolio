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
#include "Main/Character/InteractorComponent.h"
#include "Main/Character/CharacterInputComponent.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "WeaponSystem/WeaponSystemComponent.h"
#include "MainCharacter.generated.h"



UCLASS()
class CLONE1_API AMainCharacter : public ABaseCharacter, public IResourceInterface, public ICharacterEffectReceiverInterface, public IDamageableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Components")
	UWeaponComponent* WeaponSystemComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Components")
	UCharacterInputComponent* CharacterInputComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInteractorComponent* InteractionComponent;
	
	
public:

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USceneComponent* AbilityProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;
	
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
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;
	UFUNCTION()
	void ApplyAbilityVisuals(UAbilityData* Ability, FGuid InstanceID, FAbilityTargetData& Targets);
	UFUNCTION()
	void CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier);

public:	
	AMainCharacter();
	
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
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void OnWeaponInput(EWeaponInputCommand Command);
	
	
	
	// On Input
	UFUNCTION()
	void OnAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event);
	UFUNCTION()
	void OnInteractInput();
	
	
	
	UFUNCTION()
	void TryUseAbility(FGameplayTag SlotTag);
	UFUNCTION()
	void SetSkeletalDefaults(USkeletalMeshComponent* SkeletalMesh);
	UFUNCTION()
	void EquipMesh(USkeletalMesh* MeshToEquip, EEquipmentType Type);
	UFUNCTION()
	void UnEquipMesh(EEquipmentType Type);
	UFUNCTION()
	void ApplyAttributes();
	
	UFUNCTION()
	void DisplayWeaponVisuals(UWeaponInstance* WeaponInstance);
	UFUNCTION()
	UAbilitySystemComponent* GetAbilitySystem();
	UFUNCTION()
	UWeaponComponent* GetWeaponSystem();
	UFUNCTION()
	UCharacterInputComponent* GetCharacterInput();
	UFUNCTION()
	UInteractorComponent* GetInteractionComponent();
	
};

