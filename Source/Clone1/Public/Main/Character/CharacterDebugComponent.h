// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FWeaponInstanceRuntimeSnapshot.h"
#include "GameplayTagContainer.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "CharacterDebugComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourcesDebugSnapshot, FResourceDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityCastDebugSnapshot, FAbilityDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeDebugSnapshot, FAttributeDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCastFailedDebugSnapshot, EAbilityFailureReason&, Reason, FGameplayTag, AbilityName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectAppearDebugSnapshot, FAbilityDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectExpiredDebugSnapshot, FAbilityDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisplayDebugSnapshots_All, FEntityGameplayDebugSnapshot&, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponRuntimeSnapshot, FWeaponInstanceRuntimeSnapshot, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEntityDebugSnapshot,ABaseCharacter*, BaseCharacter, FEntityGameplayDebugSnapshot, Snapshot);


class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UCharacterDebugComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> Owner = nullptr;
	
public:	
	FOnAbilityCastDebugSnapshot OnAbilityCastDebugSnapshotDelegate;
	FOnResourcesDebugSnapshot OnResourcesDebugSnapshotDelegate;
	FOnCastFailedDebugSnapshot OnAbilityCastFailedDebugSnapshotDelegate;
	FOnAttributeDebugSnapshot OnAttributeDebugSnapshotDelegate;
	FOnEffectAppearDebugSnapshot OnEffectAppearDebugSnapshotDelegate;
	FOnEffectExpiredDebugSnapshot OnEffectExpiredDebugSnapshotDelegate;
	FOnDisplayDebugSnapshots_All OnDisplayDebugSnapshots_AllDelegate;
	FOnWeaponRuntimeSnapshot OnWeaponRuntimeSnapshotDelegate;
	FOnEntityDebugSnapshot OnEntityDebugSnapshotDelegate;

	
protected:
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
	UFUNCTION()
	void SendWeaponRuntimeDebugSnapshot(UWeaponInstance* WeaponInstance);
	UFUNCTION()
	void HandleWeaponDebugSnapshot_Init(UWeaponInstance* WeaponInstance);
public:
	UFUNCTION()
	void AbilityCastFailDebugSnapshot( UAbilityData* Ability, EAbilityFailureReason Reason);
	UFUNCTION()
	void AbilityCastDebugSnapshot(UAbilityData* Ability,  FGuid InstanceID, FAbilityTargetData& Targets );

	UFUNCTION()
	void Init(ABaseCharacter* InOwner);
	UFUNCTION()
	FEntityGameplayDebugSnapshot RequestEntitySnapshotData() const;
};
