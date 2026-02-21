// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character/AbilitySystemComponent.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Main/Character/EffectsComponent.h"
#include "Types/ItemTypes.h"
#include "GameplayTagContainer.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Interfaces/ResourceInterface.h"
#include "Main/Character/InteractorComponent.h"
#include "Main/Character/CharacterInputComponent.h"
#include "WeaponSystem/WeaponSystemComponent.h"
#include "MainCharacter.generated.h"



UCLASS()
class CLONE1_API AMainCharacter : public ABaseCharacter, public IResourceInterface, public ICharacterEffectReceiverInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
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
	void SetupFromPlayerState();
	
	
	// Replication
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	UFUNCTION(Server, Reliable)
	void Server_AbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event,FAbilityTargetData ClientTargetData);
public:	
	AMainCharacter();
	
	// IResource Interface
	virtual void ApplyResourceDelta_Implementation(ECharacterResource Type, float Delta) override;
	virtual float GetEnergy_Implementation() const override;
	virtual float GetHealth_Implementation() const override;
	
	
	// CharacterEffectReciever Interface
	virtual void ApplyEffect_Implementation(AActor* EffectOrigin,FCharacterEffect& Effect, FGuid SourceInstanceID) override;	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent_Implementation() override;
	
	
	// On Input
	UFUNCTION()
	void OnAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event);
	UFUNCTION()
	void OnInteractInput();
	UFUNCTION()
	void OnWeaponInput(EWeaponInputCommand Command);
	
	
	
	UFUNCTION()
	void OnAbilityCast(bool bState, FGameplayTag AbilityTag);
	virtual void OnRespondToHealthChange(float Delta) override;
	
	
	// Visuals
	UFUNCTION()
	void DisplayWeaponVisuals(UWeaponInstance* WeaponInstance);
	UFUNCTION()
	void ApplyAbilityVisuals(FGameplayTag AbilityTag, FGuid InstanceID, FVector AbilityDirection);
	UFUNCTION()
	void CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier);
    	
	
	// Mesh
	UFUNCTION()
	void SetSkeletalDefaults(USkeletalMeshComponent* SkeletalMesh);
	UFUNCTION()
	void EquipMesh(USkeletalMesh* MeshToEquip, EEquipmentType Type);
	UFUNCTION()
	void UnEquipMesh(EEquipmentType Type);
	UFUNCTION()
	void ApplyAttributes();
	
	
	UFUNCTION()
	UWeaponComponent* GetWeaponSystem();
	UFUNCTION()
	UCharacterInputComponent* GetCharacterInput();
	UFUNCTION()
	UInteractorComponent* GetInteractionComponent();
	
};

