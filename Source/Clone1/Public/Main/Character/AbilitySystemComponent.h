// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Instances/ActiveAbilityInstance.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Definitions/AbilityData.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.generated.h"




DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCasted, FGameplayTag, AbilityTag,FGuid, AbilityInstanceID, FVector, AbilityDirection); // All
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCastFail, FGameplayTag, AbilityTag, EAbilityFailureReason, Reason); // Owner
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityAborted, AActor*, AbilityOrigin, const FGuid&, AbilityInstanceID); // All
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSCastState,bool, State, FGameplayTag, AbilityTag); // All




USTRUCT()
struct FRepAbilityCast
{
	GENERATED_BODY()

	UPROPERTY() bool bIsCasting = false;
	UPROPERTY() TWeakObjectPtr<AActor> OriginActor;
	UPROPERTY() FGameplayTag AbilityTag;
	UPROPERTY() FGuid AbilityInstanceID;
	UPROPERTY() FVector AbilityDirection = FVector::ZeroVector;
	UPROPERTY() bool bAborted = false;
	UPROPERTY() int32 Counter;
};

class AMainPlayerState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TWeakObjectPtr<AMainPlayerState> Owner;
	UPROPERTY()
	TWeakObjectPtr<APawn> Avatar;
	
	UPROPERTY()
	bool bIsCasting = false;
	
	UPROPERTY()
	UActiveAbilityInstance* ActiveAbility;
	UPROPERTY()
	TMap<FPrimaryAssetId, float> OngoingCooldowns;
	UPROPERTY()
	UTargetingStrategy* ActiveTargetingStrategy = nullptr;
	
	
	// Replicated state
	UPROPERTY(Replicated)
	TArray<UAbilityData*> GrantedAbilities;
	UPROPERTY(Replicated)
	FRepAbilityCast RepAbilityCast;

public:	
	FOnStateRequest OnStateRequestDelegate;
	FOnAbilityCasted OnAbilityCastedDelegate;
	FOnAbilityCastFail OnAbilityCastFailDelegate;
	FOnAbilityAborted OnAbilityAbortedDelegate;
	FOnSCastState OnCastStateDelegate;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Immediate response for net-sync
	UFUNCTION(Client, Reliable)
	void Client_OnCastFailed(UAbilityData* Ability,EAbilityFailureReason FailureReason);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_AbilityCasted(FGameplayTag AbilityTag, FGuid AbilityInstanceID, FVector Direction);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_CastStateChanged(bool bState,FGameplayTag AbilityTag);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_AbilityAborted(AActor* OriginActor, const FGuid& AbilityInstanceID);
	
	// MAIN CHAIN
	bool Validate(UAbilityData* Ability, FAbilityTargetData& OutTargetData, EAbilityFailureReason& OutFailReason);
	UFUNCTION()
	void Commit(UAbilityData* Ability);
	void Execute(UAbilityData* Ability,FAbilityTargetData& TargetData);
	
	
	UFUNCTION()
	void TryUseAbility_Server(UAbilityData* Ability, FAbilityTargetData& ClientTargetData);
	UFUNCTION()
	void TryAbortAbility(UAbilityData* Ability);
	
	UFUNCTION()
	void TrackCooldown(UAbilityData* Ability);
public:
	UAbilitySystemComponent();
	UFUNCTION()
	void Init(AMainPlayerState* InOwner);
	UFUNCTION()
	void SetPawn(APawn* InPawn);
	
	UFUNCTION()
	void GrantAbilities(TArray<UAbilityData*> Abilities);
	UFUNCTION()
	void TryUseAbility(UAbilityData* Ability);																					// Used also by EnemyAI
	UFUNCTION()
	void ResolveAbilityInput(FGameplayTag AbilityTag,EAbilityInputEvent Event);													// System entry point 
	UFUNCTION()
	void ResolveAbilityInput_Server(FGameplayTag AbilityTag,EAbilityInputEvent Event, FAbilityTargetData& ClientTargetData);
	
	
	// GETTERS
	UFUNCTION()
	bool IsOnCooldown(UAbilityData* Ability);
	UFUNCTION()
	bool IsAbilityActive(FGameplayTag InAbilityTag) const;
	TArray<UAbilityData*>& GetGrantedAbilities();
	UFUNCTION()
	APawn* GetPawn() const;
	UFUNCTION()
	FGameplayTag GetCurrentAbilityTag();
	
	
	// Local
	UFUNCTION()
	void ResolveTargetingLocally(UAbilityData* Ability, FAbilityTargetData& OutTargetData);
};
