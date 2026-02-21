// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/AbilitySystemComponent.h"
#include "AbilityDatabaseSubsystem.h"
#include "AbilitySystem/Instances/ActiveAbilityInstance.h"
#include "GameplayTagContainer.h"
#include "Interfaces/ResourceInterface.h"
#include "Main/MyGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Main/PlayerState/MainPlayerState.h"


void UAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UAbilitySystemComponent, RepAbilityCast);
	DOREPLIFETIME_CONDITION(UAbilitySystemComponent, GrantedAbilities, COND_OwnerOnly);
}

void UAbilitySystemComponent::Client_OnCastFailed_Implementation(UAbilityData* Ability,EAbilityFailureReason FailureReason)
{
	const UEnum* EnumPtr = StaticEnum<EAbilityFailureReason>();
	FString ReasonString = EnumPtr ? EnumPtr->GetNameStringByValue((int64)FailureReason) : TEXT("Unknown");
	
	OnAbilityCastFailDelegate.Broadcast(Ability->Tag,FailureReason);
}

// Sets default values for this component's properties
UAbilitySystemComponent::UAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UAbilitySystemComponent::Init(AMainPlayerState* InOwner)
{
	if (InOwner)
	{
		Owner = InOwner;
	}
}

void UAbilitySystemComponent::SetPawn(APawn* InPawn)
{
	Avatar = InPawn;
}

APawn* UAbilitySystemComponent::GetPawn() const
{
	return Avatar.Get();
}

FGameplayTag UAbilitySystemComponent::GetCurrentAbilityTag()
{
	return ActiveAbility->AbilityTag;
}


void UAbilitySystemComponent::Multicast_CastStateChanged_Implementation(bool bState,FGameplayTag AbilityTag)
{
	OnCastStateDelegate.Broadcast(bState,AbilityTag);
}

void UAbilitySystemComponent::Multicast_AbilityAborted_Implementation(AActor* OriginActor,
	const FGuid& AbilityInstanceID)
{
	OnAbilityAbortedDelegate.Broadcast(OriginActor, AbilityInstanceID);
}

void UAbilitySystemComponent::Multicast_AbilityCasted_Implementation(FGameplayTag AbilityTag, FGuid AbilityInstanceID,
                                                                     FVector Direction)
{
	OnAbilityCastedDelegate.Broadcast(AbilityTag, AbilityInstanceID, Direction);
}


void UAbilitySystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ActiveTargetingStrategy)
	{
		ActiveTargetingStrategy->TickTargeting(DeltaTime);
	}
}


void UAbilitySystemComponent::ResolveAbilityInput(FGameplayTag AbilityTag,EAbilityInputEvent Event)
{
	if (!GetPawn()->HasAuthority()) return;
	
	APawn* Pawn = GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC ResolveAbilityInput: Pawn is NULL. Owner=%s"), *GetNameSafe(GetOwner()));
		return;
	}

	UAbilityDatabaseSubsystem* DB =
		Pawn->GetGameInstance<UMyGameInstance>()->GetSubsystem<UAbilityDatabaseSubsystem>();

	if (!DB)
	{
		UE_LOG(LogTemp, Error, TEXT("ASC: AbilityDatabaseSubsystem is NULL"));
		return;
	}

	UAbilityData* AbilityData = DB->GetAbilityByTag(AbilityTag);
	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC: No AbilityData for tag %s"), *AbilityTag.ToString());
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ASC: Found ability %s Event=%d"), *GetNameSafe(AbilityData), (int32)Event);
	
	
	
	if (!AbilityData) return;
	

	switch (Event)
	{
	case  EAbilityInputEvent::Completed:
		{
			TryAbortAbility(AbilityData);			
			break;
		}
	case  EAbilityInputEvent::Started:
		{
			TryUseAbility(AbilityData);
			break;
		}
	}
}

void UAbilitySystemComponent::ResolveAbilityInput_Server(FGameplayTag AbilityTag, EAbilityInputEvent Event,
	FAbilityTargetData& ClientTargetData)
{
	if (!GetPawn()->HasAuthority()) return;
	
	APawn* Pawn = GetPawn();
	if (!Pawn)
		return;

	UAbilityDatabaseSubsystem* DB =
		Pawn->GetGameInstance<UMyGameInstance>()->GetSubsystem<UAbilityDatabaseSubsystem>();

	if (!DB)
		return;

	UAbilityData* AbilityData = DB->GetAbilityByTag(AbilityTag);
	if (!AbilityData)
		return;
	
	if (!AbilityData) return;
	
	switch (Event)
	{
	case  EAbilityInputEvent::Completed:
		{
			TryAbortAbility(AbilityData);			
			break;
		}
	case  EAbilityInputEvent::Started:
		{
			TryUseAbility_Server(AbilityData,ClientTargetData);
			break;
		}
	}
}

void UAbilitySystemComponent::TryUseAbility(UAbilityData* Ability)
{
	
	if (bIsCasting)
		return;
	
	EAbilityFailureReason FailureReason = EAbilityFailureReason::None;
	FAbilityTargetData TargetData;
	if (Validate(Ability, TargetData, FailureReason))
	{
		Commit(Ability);
		Execute(Ability, TargetData);
	}
	else
	{
		Client_OnCastFailed(Ability, FailureReason);
	}
}

void UAbilitySystemComponent::TryUseAbility_Server(UAbilityData* Ability, FAbilityTargetData& ClientTargetData)
{
	if (!GetPawn()->HasAuthority()) return;
	
	if (bIsCasting)
		return;
	
	EAbilityFailureReason FailureReason = EAbilityFailureReason::None;
	if (Validate(Ability, ClientTargetData, FailureReason))
	{
		Commit(Ability);
		Execute(Ability, ClientTargetData);
	}
	else
	{
		Client_OnCastFailed(Ability, FailureReason);
	}
}

void UAbilitySystemComponent::TryAbortAbility(UAbilityData* Ability)
{
	if (!GetPawn()->HasAuthority()) return;
	
	
	if (!ActiveAbility)
		return;

	if (Ability->AbilityCastMode != EAbilityCastMode::Casted)
		return;

	const FGuid InstanceID = ActiveAbility->AbilityInstanceID;

	// --- Immediate visuals ---
	Multicast_AbilityAborted(GetPawn(), InstanceID);

	OnCastStateDelegate.Broadcast(false,ActiveAbility->AbilityTag);
	
	// --- Stop runtime cast ---
	ActiveAbility->CancelCast();
	ActiveAbility = nullptr;
	bIsCasting = false;

	// --- Replicated state ---
	RepAbilityCast.bAborted = true;
	RepAbilityCast.bIsCasting = false;
	RepAbilityCast.AbilityInstanceID = InstanceID;
	RepAbilityCast.OriginActor = GetPawn();
	RepAbilityCast.Counter++;

	
	OnStateRequestDelegate.ExecuteIfBound(EEntityState::Idle);

	GetOwner()->ForceNetUpdate();


}

void UAbilitySystemComponent::GrantAbilities(TArray<UAbilityData*> Abilities)
{
	GrantedAbilities = Abilities;
}

bool UAbilitySystemComponent::IsOnCooldown(UAbilityData* Ability)
{
	if (!Ability)
		return false;
	
	FPrimaryAssetId AssetID = Ability->GetPrimaryAssetId();
	if (!AssetID.IsValid())
		return false;
	
	float* EndTime = OngoingCooldowns.Find(AssetID);
	if (!EndTime) 
		return false;
	
	return GetWorld()->GetTimeSeconds() < *EndTime;
}



bool UAbilitySystemComponent::Validate(UAbilityData* Ability, FAbilityTargetData& OutTargetData, EAbilityFailureReason& OutFailReason)
{
	if (!Ability)
		return false;
	
	float AvailableEnergy = IResourceInterface::Execute_GetEnergy(GetPawn());
	if (AvailableEnergy < Ability->EnergyCost)
	{
		OutFailReason = EAbilityFailureReason::InsufficientEnergy;
		return false;
	}
	
	if (IsOnCooldown(Ability))
	{
		OutFailReason = EAbilityFailureReason::OnCooldown;
		return false;
	}
	if (GetOwner()->HasAuthority())
	{
		if (!GrantedAbilities.Contains(Ability))
			return false;
	}
	
	// If targeting is passed, and its valid, insta-validate. 
	// Ideally we should perform some more precise validity checks on the server side to confirm client's target data.
	if (OutTargetData.bHasValidTarget)
	{
		return true;
	}
		
	//-- RESOLVE TARGETING (STATE MACHINE)
	if (!ActiveTargetingStrategy ||
		ActiveTargetingStrategy->GetClass() != Ability->TargetingStrategy)
	{
		if (ActiveTargetingStrategy)
		{
			ActiveTargetingStrategy->AbortTargeting();
			ActiveTargetingStrategy = nullptr;
		}

		ActiveTargetingStrategy =
			NewObject<UTargetingStrategy>(this, Ability->TargetingStrategy);
	}
	
	
	 
	EAbilityTargetingStatus Status = ActiveTargetingStrategy->TryResolveTargeting(GetPawn(),Ability->IndicatorData, Ability->CustomTargetingRules, OutTargetData); 
	switch (Status)
	{
		case EAbilityTargetingStatus::Invalid:
		{
			ActiveTargetingStrategy = nullptr;
			OutFailReason = EAbilityFailureReason::InvalidTarget;
			return false;
		}
		case EAbilityTargetingStatus::InProgress:
		{
			OutFailReason = EAbilityFailureReason::TargetingInProgress;		
			return false;
		}
		case EAbilityTargetingStatus::Confirmed:
		{
			ActiveTargetingStrategy = nullptr;
			return true;
		}
	}
	return false;
}

void UAbilitySystemComponent::Commit(UAbilityData* Ability)
{
	
	IResourceInterface::Execute_ApplyResourceDelta(GetPawn(),ECharacterResource::Energy,-Ability->EnergyCost);
}

void UAbilitySystemComponent::Execute(UAbilityData* Ability,FAbilityTargetData& InTargetData)
{
	// Create runtime instance
	UActiveAbilityInstance* NewActiveAbilityInstance = NewObject<UActiveAbilityInstance>(this);
	NewActiveAbilityInstance->AbilityTag = Ability->Tag;
	NewActiveAbilityInstance->AbilityInstanceID = FGuid::NewGuid();
	NewActiveAbilityInstance->StartCast(Ability->CastTime);
	
	// Set State
	ActiveAbility = NewActiveAbilityInstance;
	ActiveAbility->OnCastFinishedDelegate.AddLambda([this]()
	{
		Multicast_CastStateChanged(false,ActiveAbility->AbilityTag);
		ActiveAbility = nullptr;
		bIsCasting = false;
		
		OnStateRequestDelegate.ExecuteIfBound(EEntityState::Idle);
		
		RepAbilityCast.bIsCasting = false;
		RepAbilityCast.Counter++;
	});
	
	
	bIsCasting = true;
	
	// Replicate
	RepAbilityCast.bIsCasting = true;
	RepAbilityCast.AbilityTag = NewActiveAbilityInstance->AbilityTag;
	RepAbilityCast.bAborted = false;
	RepAbilityCast.AbilityInstanceID = NewActiveAbilityInstance->AbilityInstanceID;
	RepAbilityCast.AbilityDirection = InTargetData.Direction;
	RepAbilityCast.OriginActor = GetPawn();
	RepAbilityCast.Counter++;
	
	
	// // Broadcast
	Multicast_CastStateChanged(true,ActiveAbility->AbilityTag);
	Multicast_AbilityCasted(NewActiveAbilityInstance->AbilityTag, NewActiveAbilityInstance->AbilityInstanceID,InTargetData.Direction);
	
	
	OnStateRequestDelegate.ExecuteIfBound(EEntityState::Casting);
	
	if (Ability->Effects.Num() == 0)
		return;

	int32 EffectCount = Ability->Effects.Num();
	int32 SuccessCount = 0;
	
	for (TSubclassOf<UAbilityEffect> EffectClass : Ability->Effects)
	{
		if (!EffectClass) continue;
		
		UAbilityEffect* Effect = NewObject<UAbilityEffect>(this, EffectClass);
	
		if (Effect->TryApplyEffect(
			GetPawn(),
			InTargetData,
			NewActiveAbilityInstance->AbilityInstanceID
		))
		{
			SuccessCount++;
		}
	}
	if (SuccessCount > 0)
	{
		TrackCooldown(Ability);
	}
	else
	{
		// Return energy cost and abort ability if no effect succeeded
		IResourceInterface::Execute_ApplyResourceDelta(GetPawn(),ECharacterResource::Energy,Ability->EnergyCost);
		TryAbortAbility(Ability);
		ActiveTargetingStrategy = nullptr;
		Client_OnCastFailed(Ability,EAbilityFailureReason::InvalidTarget);
	}
		
}

void UAbilitySystemComponent::ResolveTargetingLocally(UAbilityData* Ability, FAbilityTargetData& OutTargetData)
{
	//-- RESOLVE TARGETING (STATE MACHINE)
	if (!ActiveTargetingStrategy ||
		ActiveTargetingStrategy->GetClass() != Ability->TargetingStrategy)
	{
		if (ActiveTargetingStrategy)
		{
			ActiveTargetingStrategy->AbortTargeting();
			ActiveTargetingStrategy = nullptr;
		}

		ActiveTargetingStrategy =
			NewObject<UTargetingStrategy>(this, Ability->TargetingStrategy);
	}
	
	ActiveTargetingStrategy->TryResolveTargeting(GetPawn(),Ability->IndicatorData, Ability->CustomTargetingRules, OutTargetData); 
}

void UAbilitySystemComponent::TrackCooldown(UAbilityData* Ability)
{
	if (!GetPawn()->HasAuthority()) return;
	
	UWorld* World = GetPawn() ? GetPawn()->GetWorld() : nullptr;
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("TrackCooldown: World is NULL"));
		return;
	}

	float EndTime = World->GetTimeSeconds() + Ability->Cooldown;
	FPrimaryAssetId AssetID = Ability->GetPrimaryAssetId();
	
	OngoingCooldowns.Add(AssetID, EndTime);
}

bool UAbilitySystemComponent::IsAbilityActive(FGameplayTag InAbilityTag) const
{
	if (ActiveAbility == nullptr)
		return false;

	return ActiveAbility->AbilityTag == InAbilityTag;
}

TArray<UAbilityData*>& UAbilitySystemComponent::GetGrantedAbilities()
{
	return GrantedAbilities;
}
