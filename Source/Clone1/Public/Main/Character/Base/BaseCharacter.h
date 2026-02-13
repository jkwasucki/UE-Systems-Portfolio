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
#include "Main/Character/CharacterDebugComponent.h"
#include "Types/CharacterTypes.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChange, EEntityState, State);


class UCharacterMovementComponent;
UCLASS()
class CLONE1_API ABaseCharacter : public ACharacter, public IDebugInfoProviderInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	bool bIsAlive = true;
public:
	FOnStateChange OnStateChangeDelegate;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	EEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityState EntityState = EEntityState::Idle;
	

	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Components")
	UResourceComponent* ResourceComponent;										// Health / Energy
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Components") 
	UCharacterMoverComponent* MoverComponent;									// Movement overrides
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components")
	UCharacterVFXComponent* VFXComponent;										// VFX Effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components")
	UCharacterAnimationComponent* AnimationComponent;							// Animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Components")
	UAttributesComponent* AttributesComponent;									// Attribute's modifiers
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Components")
	UEffectsComponent* EffectsComponent;										// Character Effects 
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Components")
	UCharacterDebugComponent* DebugComponent;
	

protected:
	virtual void BeginPlay() override;
public:
	ABaseCharacter();
	
	UFUNCTION()
	void CleanupVisuals(const FGuid& Identifier);
	UFUNCTION()
	void Die();
	UFUNCTION()
	void Resurrect();
	
	UFUNCTION()
	virtual void OnRespondToHealthChange(float Delta);

	UFUNCTION()
	void SetState(EEntityState State);
	
	
	// GETTERS
	virtual FEntityGameplayDebugSnapshot GetDebugInfo_Implementation() override;
	UFUNCTION(BlueprintPure)
	UCharacterAnimationComponent* BP_GetCharacterAnimationComponent() const;
	UFUNCTION()
	UAttributesComponent* GetAttributesComponent();
	UFUNCTION()
	UEffectsComponent* GetEffectsComponent();
	UFUNCTION()
	UResourceComponent* GetResourceComponent();
	UFUNCTION()
	UCharacterDebugComponent* GetDebugComponent();
	UFUNCTION()
	UCharacterAnimationComponent* GetAnimationComponent();
	UFUNCTION()
	UCharacterVFXComponent* GetVFXComponent();
	UFUNCTION()
	UCharacterMoverComponent* GetMoverComponent();
	UFUNCTION()
	EEntityState  GetState();
	UFUNCTION()
	bool IsEnemy();
	UFUNCTION()
	bool IsAlly();
	UFUNCTION()
	bool IsAlive();
	
};
