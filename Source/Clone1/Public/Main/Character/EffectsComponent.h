// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Instances/ActiveEffectInstance.h"
#include "Components/ActorComponent.h"
#include "Structs/FCharacterEffect.h"
#include "EffectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectStart,FCharacterEffect, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConsumableEffectStart,FCharacterEffect, Effect, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEffectsComponent_OnEffectEnd,FCharacterEffect, Effect, FGuid, EffectInstanceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectExtended,UActiveEffectInstance*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestVFX,FVFXData, VFX, const FGuid, EffectInstanceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestVFXEnd,const FGuid&, EffectInstanceID);


USTRUCT()
struct  FRepActiveEffect
{
	GENERATED_BODY()
	
	UPROPERTY() FGuid EffectInstanceID;
	UPROPERTY() FGuid SourceInstanceID;
	UPROPERTY() float StartTime;
	UPROPERTY() FCharacterEffect EffectDefinition;
	UPROPERTY() int32 Counter = 0;
};


class UInventoryComponent;
class UAttributesComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UEffectsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	TWeakObjectPtr<UAttributesComponent> AttributesComponent = nullptr;
	
	UPROPERTY()
	TArray<UActiveEffectInstance*> ActiveEffects;
	TArray<FRepActiveEffect> CachedEffects;									// Used to compare Old data vs New
	
	

	UPROPERTY(ReplicatedUsing=OnRep_ActiveEffects)
	TArray<FRepActiveEffect> RepEffects;

public:	
	UPROPERTY()
	FOnConsumableEffectStart OnConsumableEffectStartDelegate;
	UPROPERTY()
	FOnEffectStart OnEffectStartDelegate;
	UPROPERTY()
	FEffectsComponent_OnEffectEnd OnEffectEndDelegate;
	UPROPERTY()
	FOnEffectExtended OnEffectExtendedDelegate;
	FOnRequestVFX OnRequestVFXDelegate;
	FOnRequestVFXEnd OnRequestVFXEndDelegate;
	
	
	UPROPERTY(EditAnywhere)
	UDataTable* ItemsDataTable;
	
protected:
	UFUNCTION()
	void OnRep_ActiveEffects();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	UFUNCTION()
	void ApplyConsumableEffect(AActor* EffectOrigin,FCharacterEffect& Effect,FGuid AbilityInstanceID, FName ItemID);
	UFUNCTION()
	TArray<FGuid> RemoveEffects(AActor* EffectOrigin,const FGuid& SourceInstanceID, bool bFailure);
	UFUNCTION()
	void OnAbilityAborted(AActor* EffectOrigin,const FGuid& SourceInstanceID);
	UFUNCTION()
	void ExternalOriginAborted(AActor* EffectOrigin,const FGuid& SourceInstanceID);
	UFUNCTION()
	void HandleEffectEnded(UActiveEffectInstance* EffectInstance);
public:
	UEffectsComponent();
	
	UFUNCTION()
	void ApplyEffect(AActor* EffectOrigin,FCharacterEffect& Effect,FGuid AbilityInstanceID);
	
	
	UFUNCTION()
	void SetAttributesComponentLink(UAttributesComponent* inAttributesComponent);
	UFUNCTION()
	void SetInventoryComponentLink(UInventoryComponent* inInventoryComponent);
	UFUNCTION()
	void SetAbilitySystemComponentLink(UAbilitySystemComponent* InAbilitySystemComponent);
	
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
};
