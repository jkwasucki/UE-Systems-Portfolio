// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Instances/ActiveEffectInstance.h"
#include "Components/ActorComponent.h"
#include "Structs/FCharacterEffect.h"
#include "EffectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectStart,UActiveEffectInstance*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectsComponent_OnEffectEnd,UActiveEffectInstance*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectExtended,UActiveEffectInstance*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestVFX,FVFXData&, VFX, const FGuid&, EffectInstanceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestVFXEnd,const FGuid&, EffectInstanceID);

class UInventoryComponent;
class UAttributesComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEffectsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<UActiveEffectInstance*> ActiveEffects;
public:	
	
	UPROPERTY()
	UAttributesComponent* AttributesComponent = nullptr;
	
	
	FOnEffectStart OnEffectStartDelegate;
	FEffectsComponent_OnEffectEnd OnEffectEndDelegate;
	FOnEffectExtended OnEffectExtendedDelegate;
	FOnRequestVFX OnRequestVFXDelegate;
	FOnRequestVFXEnd OnRequestVFXEndDelegate;
	
	
	
	UPROPERTY(EditAnywhere)
	UDataTable* ItemsDataTable;
	
	UFUNCTION()
	void ApplyEffect(AActor* EffectOrigin,FCharacterEffect& Effect,FGuid AbilityInstanceID);
	UFUNCTION()
	TArray<FGuid> RemoveEffects(AActor* EffectOrigin,const FGuid& SourceInstanceID, bool bFailure);
	UFUNCTION()
	void OnAbilityAborted(AActor* EffectOrigin,const FGuid& SourceInstanceID);
	UFUNCTION()
	void ExternalOriginAborted(AActor* EffectOrigin,const FGuid& SourceInstanceID);
	UFUNCTION()
	void HandleEffectEnded(UActiveEffectInstance* EffectInstance);
	
	
	// GETTERS
	UFUNCTION()
	bool IsEffectActiveByInstance(FGuid EffectInstanceID);
	UFUNCTION()
	bool IsEffectActiveByTypeID(FName TypeID);
	UFUNCTION()
	float GetDurationForEffect(FGuid EffectInstanceID);
	UFUNCTION()
	TArray<UActiveEffectInstance*> GetConsumableEffects() const;	
	UFUNCTION()
	TArray<FCharacterEffect> GetActiveEffectsDefinitions();
	//
	UActiveEffectInstance* GetEffectByID(FGuid EffectInstanceID);
	
	
	
	UFUNCTION()
	void SetAttributesComponentLink(UAttributesComponent* inAttributesComponent);
	UFUNCTION()
	void SetInventoryComponentLink(UInventoryComponent* inInventoryComponent);
	UFUNCTION()
	void SetAbilitySystemComponentLink(UAbilitySystemComponent* InAbilitySystemComponent);
};
