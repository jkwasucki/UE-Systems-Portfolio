// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Instances/ActiveAbilityInstance.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Definitions/AbilityData.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCast, UAbilityData*, Ability,FGuid, AbilityInstanceID, FAbilityTargetData&, Targets );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCastFail, UAbilityData*, Ability, EAbilityFailureReason, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityAborted, AActor*, AbilityOrigin, const FGuid&, AbilityInstanceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginCast,bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndCast,bool, State);




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilitySystemComponent();

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<UAbilityData*> GrantedAbilities;

	UPROPERTY()
	bool bIsCasting = false;
	UPROPERTY()
	UActiveAbilityInstance* ActiveAbility;
	UPROPERTY()
	TMap<FPrimaryAssetId, float> OngoingCooldowns;
	UPROPERTY()
	UTargetingStrategy* ActiveTargetingStrategy = nullptr;
public:	
	FOnStateRequest OnStateRequestDelegate;
	FOnAbilityCast OnAbilityCastDelegate;
	FOnAbilityCastFail OnAbilityCastFailDelegate;
	FOnAbilityAborted OnAbilityAbortedDelegate;
	FOnBeginCast OnBeginCastDelegate;
	FOnEndCast OnEndCastDelegate;
	
	
	// GETTERS
	UFUNCTION()
	bool IsOnCooldown(UAbilityData* Ability);
	UFUNCTION()
	bool IsAbilityActive(FGameplayTag AbilityTag) const;
	TArray<UAbilityData*>& GetGrantedAbilities();
	UFUNCTION()
	void TryUseAbility(UAbilityData* Ability);				// INITIALIZER
	UFUNCTION()
	void TryAbortAbility(UAbilityData* Ability);
	UFUNCTION()
	void GrantAbilities(TArray<UAbilityData*> Abilities);
	UFUNCTION()
	void TrackCooldown(UAbilityData* Ability);
	
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	// MAIN CHAIN
	bool Validate(UAbilityData* Ability, FAbilityTargetData& OutTargetData, EAbilityFailureReason& OutFailReason);
	UFUNCTION()
	void Commit(UAbilityData* Ability);
	void Execute(UAbilityData* Ability,FAbilityTargetData& TargetData);
};
