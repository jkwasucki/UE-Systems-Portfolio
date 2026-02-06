// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/Character/CharacterAnimationComponent.h"
#include "Main/Character/ResourceComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/DebugInfoProviderInterface.h"
#include "Main/Character/AttributesComponent.h"
#include "Main/Character/CharacterVFXComponent.h"
#include "Main/Character/CharacterMoverComponent.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "Types/CharacterTypes.h"
#include "BaseCharacter.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChange, EEntityState, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourcesDebugSnapshot, FResourceDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityCastDebugSnapshot, FAbilityDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeDebugSnapshot, FAttributeDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCastFailedDebugSnapshot, EAbilityFailureReason&, Reason, FGameplayTag, AbilityName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectAppearDebugSnapshot, FAbilityDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectExpiredDebugSnapshot, FAbilityDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplayDebugSnapshots_All, FEntityGameplayDebugSnapshot&, Snapshot);
class UCharacterMovementComponent;
UCLASS()
class CLONE1_API ABaseCharacter : public ACharacter, public IDebugInfoProviderInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	bool bIsAlive = true;
	virtual void BeginPlay() override;
	
public:
	// Sets default values for this character's properties
	ABaseCharacter();
	
	FOnStateChange OnStateChangeDelegate;
	FOnAbilityCastDebugSnapshot OnAbilityCastDebugSnapshotDelegate;
	FOnResourcesDebugSnapshot OnResourcesDebugSnapshotDelegate;
	FOnCastFailedDebugSnapshot OnAbilityCastFailedDebugSnapshotDelegate;
	FOnAttributeDebugSnapshot OnAttributeDebugSnapshotDelegate;
	FOnEffectAppearDebugSnapshot OnEffectAppearDebugSnapshotDelegate;
	FOnEffectExpiredDebugSnapshot OnEffectExpiredDebugSnapshotDelegate;
	FOnDisplayDebugSnapshots_All OnDisplayDebugSnapshots_AllDelegate;
	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	EEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityState EntityState = EEntityState::Idle;
	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UResourceComponent* ResourceComponent;						// Health / Energy
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly) 
	UCharacterMoverComponent* CharacterMoverComponent;			// Movement overrides
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterVFXComponent* CharacterVFXComponent;				// VFX Effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCharacterAnimationComponent* CharacterAnimationComponent;	// Animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAttributesComponent* AttributesComponent;					// Attribute's modifiers
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UEffectsComponent* EffectsComponent;						// Character Effects 
	
	
	
	
	
	UFUNCTION()
	void Die();
	UFUNCTION()
	void Resurrect();

	
	
	
	UFUNCTION()
	virtual void OnRespondToHealthChange(float Delta);
	
	
	// DEBUG SNAPSHOTS
	UFUNCTION()
	void ResourcesChangeDebugSnapshot();
	UFUNCTION()
	void AttributesChangeDebugSnapshot();
	UFUNCTION()
	void EffectExpiredDebugSnapshot(UActiveEffectInstance* EffectInstance);
	UFUNCTION()
	void EffectAppearedDebugSnapshot(UActiveEffectInstance* EffectInstance);
	UFUNCTION()
	void RequestDebugSnapshots();
	UFUNCTION()
	void ListenForDebugSnapshots();
	
	// GETTERS
	virtual FEntityGameplayDebugSnapshot GetDebugInfo_Implementation() override;
	
	UFUNCTION(BlueprintPure)
	UCharacterAnimationComponent* BP_GetCharacterAnimationComponent() const;
	UFUNCTION()
	FEntityGameplayDebugSnapshot RequestEntitySnapshotData() const;
	UFUNCTION()
	UAttributesComponent* GetAttributesComponent();
	UFUNCTION()
	UEffectsComponent* GetEffectsComponent();
	UFUNCTION()
	UResourceComponent* GetResourceComponent();
	UFUNCTION()
	UCharacterAnimationComponent* GetCharacterAnimationComponent();
	UFUNCTION()
	UCharacterVFXComponent* GetCharacterVFXComponent();
	UFUNCTION()
	UCharacterMoverComponent* GetCharacterMoverComponent();
	UFUNCTION()
	bool IsEnemy();
	UFUNCTION()
	bool IsAlly();
	UFUNCTION()
	bool IsAlive();
	
	
	UFUNCTION()
	void SetState(EEntityState State);
	
	

};
