// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/CharacterDebugComponent.h"

#include "AbilityDatabaseSubsystem.h"
#include "Main/MyGameInstance.h"
#include "Main/Character/Base/BaseCharacter.h"
#include "Main/Character/EffectsComponent.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "Structs/FGameplayDebugSnapshot.h"
#include "WeaponSystem/Instances/WeaponInstance.h"




void UCharacterDebugComponent::Init(ABaseCharacter* InOwner)
{
	if (!IsValid(InOwner)) return;
	
	Owner = InOwner;
	ListenForDebugSnapshots();
	RequestDebugSnapshots();
}

FEntityGameplayDebugSnapshot UCharacterDebugComponent::RequestEntitySnapshotData() const
{
	if (!Owner.IsValid()) return FEntityGameplayDebugSnapshot();
	
	FEntityGameplayDebugSnapshot Snapshot;

	// Entity info
	Snapshot.EntityType  = Owner->EntityType;
	Snapshot.EntityState = Owner->EntityState;

	// Attributes
	Snapshot.AttributeSnapshot.Attack =
		Owner->GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Attack);

	Snapshot.AttributeSnapshot.Speed =
		Owner->GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Speed);

	Snapshot.AttributeSnapshot.Armor =
		Owner->GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Armor);

	Snapshot.AttributeSnapshot.Health =
		Owner->GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Health);

	// Resources
	Snapshot.ResourceDebugSnapshot.Health =
		Owner->GetResourceComponent()->GetHealth();

	Snapshot.ResourceDebugSnapshot.Energy =
		Owner->GetResourceComponent()->GetEnergy();
	Snapshot.AbilityDebugSnapshot.Effects = Owner->GetEffectsComponent()->GetActiveEffectsDefinitions();
	
	return Snapshot;
}

void UCharacterDebugComponent::RequestDebugSnapshots()
{
	if (!Owner.IsValid()) return;
	FEntityGameplayDebugSnapshot Snapshot = RequestEntitySnapshotData();
	OnDisplayDebugSnapshots_AllDelegate.Broadcast(Snapshot);
}

void UCharacterDebugComponent::SendWeaponRuntimeDebugSnapshot(UWeaponInstance* WeaponInstance)
{
	if (!Owner.IsValid()) return;
	
	FWeaponInstanceRuntimeSnapshot RuntimeSnapshot;
	RuntimeSnapshot.InstanceID = WeaponInstance->GetInstanceID();
	RuntimeSnapshot.WeaponState = WeaponInstance->GetWeaponState();
	
	UWeaponDefinition* Definition = WeaponInstance->GetWeaponDefinition().Get();
	RuntimeSnapshot.bDefinitionExists = Definition ? true : false;
	RuntimeSnapshot.Definition = Definition;
	
	APawn* Pawn = WeaponInstance->GetOwner().Get();
	RuntimeSnapshot.bOwnerExists = Pawn ? true : false;
	RuntimeSnapshot.WeaponFireMode = WeaponInstance->GetFireMode();
	
	OnWeaponRuntimeSnapshotDelegate.Broadcast(RuntimeSnapshot);
}

void UCharacterDebugComponent::HandleWeaponDebugSnapshot_Init(UWeaponInstance* WeaponInstance)
{
	if (!Owner.IsValid()) return;
	if (!IsValid(WeaponInstance)) return;
	
	SendWeaponRuntimeDebugSnapshot(WeaponInstance);
	WeaponInstance->OnWeaponRuntimeUpdatedDelegate.RemoveDynamic(this,&UCharacterDebugComponent::SendWeaponRuntimeDebugSnapshot);
	WeaponInstance->OnWeaponRuntimeUpdatedDelegate.AddDynamic(this,&UCharacterDebugComponent::SendWeaponRuntimeDebugSnapshot);
}



// DEBUG SNAPSHOTS
void UCharacterDebugComponent::ListenForDebugSnapshots()
{
	if (!Owner.IsValid()) return;
	Owner->GetResourceComponent()->SnapshotOnHealthChangedDelegate.AddDynamic(this, &UCharacterDebugComponent::ResourcesChangeDebugSnapshot);
	Owner->GetResourceComponent()->SnapshotOnEnergyChangedDelegate.AddDynamic(this, &UCharacterDebugComponent::ResourcesChangeDebugSnapshot);
	Owner->GetAttributesComponent()->OnAttributesChangedDelegate.AddDynamic(this, &UCharacterDebugComponent::AttributesChangeDebugSnapshot);
	Owner->GetEffectsComponent()->OnEffectEndDelegate.AddDynamic(this, &UCharacterDebugComponent::EffectExpiredDebugSnapshot);
	Owner->GetEffectsComponent()->OnEffectStartDelegate.AddDynamic(this, &UCharacterDebugComponent::EffectAppearedDebugSnapshot);
}

void UCharacterDebugComponent::ResourcesChangeDebugSnapshot()
{
	if (!Owner.IsValid()) return;
	FResourceDebugSnapshot Snapshot = RequestEntitySnapshotData().ResourceDebugSnapshot;
	OnResourcesDebugSnapshotDelegate.Broadcast(Snapshot);
}
void UCharacterDebugComponent::AttributesChangeDebugSnapshot()
{
	if (!Owner.IsValid()) return;
	FAttributeDebugSnapshot Snapshot = RequestEntitySnapshotData().AttributeSnapshot;
	OnAttributeDebugSnapshotDelegate.Broadcast(Snapshot);
}
void UCharacterDebugComponent::EffectExpiredDebugSnapshot(FCharacterEffect EffectData, FGuid EffectInstanceID)
{
	if (!Owner.IsValid()) return;
	FAbilityDebugSnapshot Snapshot;
	Snapshot.Effects.Add(EffectData);
	
	OnEffectExpiredDebugSnapshotDelegate.Broadcast(Snapshot);
}
void UCharacterDebugComponent::EffectAppearedDebugSnapshot(FCharacterEffect EffectData)
{
	if (!Owner.IsValid()) return;
	FAbilityDebugSnapshot Snapshot;
	Snapshot.Effects.Add(EffectData);
	
	OnEffectAppearDebugSnapshotDelegate.Broadcast(Snapshot);
}

void UCharacterDebugComponent::AbilityCastDebugSnapshot(FGameplayTag AbilityTag, FGuid InstanceID, FVector AbilityDirection)
{
	UMyGameInstance*  GI = Owner->GetGameInstance<UMyGameInstance>();
	if (!GI) return;
	
	UAbilityDatabaseSubsystem* AbilityDatabase = GI->GetSubsystem<UAbilityDatabaseSubsystem>();
	if (!AbilityDatabase) return;
	
	UAbilityData* Ability = AbilityDatabase->GetAbilityByTag(AbilityTag);
	if (!Ability) return;
	FAbilityDebugSnapshot Snapshot;
	
	Snapshot.AbilityInstanceID = InstanceID;
	Snapshot.Cooldown = Ability->Cooldown;
	Snapshot.EnergyCost = Ability->EnergyCost;
	Snapshot.AnimationMode = Ability->AnimationData.AnimationMode;
	Snapshot.bLocksMovement = Ability->AnimationData.bBlocksMovement;
	Snapshot.TargetingStrategy = Ability->TargetingStrategy->GetName();
	Snapshot.AbilityTag = Ability->Tag;
	Snapshot.AbilityCastMode = Ability->AbilityCastMode;
	
	if (Ability->Effects.Num() > 0)
	{
		for (TSubclassOf<UAbilityEffect> BaseEffectClass : Ability->Effects)
		{
			if (!BaseEffectClass) continue;
			UAbilityEffect* EffectCDO = BaseEffectClass->GetDefaultObject<UAbilityEffect>();
			Snapshot.Effects.Add(EffectCDO->EffectData);
			
			if (EffectCDO->GetInnerEffects().Num() > 0)
			{
				for (TSubclassOf<UAbilityEffect> OnHitEffectClass : EffectCDO->GetInnerEffects())
				{
					if (!OnHitEffectClass) continue;
					UAbilityEffect* EffectCDO2 = OnHitEffectClass->GetDefaultObject<UAbilityEffect>();
					Snapshot.Effects.Add(EffectCDO2->EffectData);
				}
			}
		}
	}
	
	
	
	OnAbilityCastDebugSnapshotDelegate.Broadcast(Snapshot);
}
void UCharacterDebugComponent::AbilityCastFailDebugSnapshot(FGameplayTag AbilityTag, EAbilityFailureReason Reason)
{
	UMyGameInstance*  GI = Owner->GetGameInstance<UMyGameInstance>();
	if (!GI) return;
	
	UAbilityDatabaseSubsystem* AbilityDatabase = GI->GetSubsystem<UAbilityDatabaseSubsystem>();
	if (!AbilityDatabase) return;
	
	UAbilityData* Ability = AbilityDatabase->GetAbilityByTag(AbilityTag);
	if (!Ability) return;
	
	
	const FText ReasonText =
		StaticEnum<EAbilityFailureReason>()
			->GetDisplayNameTextByValue(
				static_cast<int64>(Reason)
			);

	OnAbilityCastFailedDebugSnapshotDelegate.Broadcast(Reason,Ability->Tag);
}
