// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/Derived/MainCharacter.h"

#include "Main/Character/CharacterVFXComponent.h"
#include "Main/Character/CharacterMoverComponent.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "Equipment/EquipmentComponent.h"
#include "Main/Character/AttributesComponent.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "Structs/FAnimationTags.h"
#include "Structs/FGameplayDebugSnapshot.h"

AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());

	ChestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(GetMesh());

	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMesh"));
	LegsMesh->SetupAttachment(GetMesh());

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMesh"));
	HandsMesh->SetupAttachment(GetMesh());

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMesh->SetupAttachment(GetMesh());

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	
	AbilityProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AbilityProjectileSpawnPoint"));

	
	SetSkeletalDefaults(HeadMesh);
	SetSkeletalDefaults(ChestMesh);
	SetSkeletalDefaults(LegsMesh);
	SetSkeletalDefaults(HandsMesh);
	SetSkeletalDefaults(FeetMesh);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilityProjectileSpawnPoint->AttachToComponent(
		GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);
	
	if (AMainPlayerState* PS = GetPlayerState<AMainPlayerState>())
	{
		if (AMainPlayerController* PC = Cast<AMainPlayerController>( PS->GetPlayerController()))
		{
			PC->InputHandlerComponent->OnAbilityInputDelegate.AddDynamic(this, &AMainCharacter::ResolveAbilityInput);
		}
		
		if (PS->InventoryComponent)
		{
			PS->InventoryComponent->OnWeightChangeDelegate.AddDynamic(GetCharacterMoverComponent(), &UCharacterMoverComponent::ModifySpeedByWeight);
			PS->EquipmentComponent->OnItemEquippedDelegate.AddDynamic(this, &AMainCharacter::EquipMesh);
			PS->EquipmentComponent->OnItemUnequippedDelegate.AddDynamic(this,  &AMainCharacter::UnEquipMesh);
		}
		
		
		
		GetAttributesComponent()->SetEquipmentComponentLink(PS->EquipmentComponent);
		GetAttributesComponent()->OnAttributesChangedDelegate.AddDynamic(this, &AMainCharacter::ApplyAttributes);
		
		// SET COMPONENT LINKS
		GetEffectsComponent()->SetInventoryComponentLink(PS->InventoryComponent);
		GetEffectsComponent()->SetAttributesComponentLink(GetAttributesComponent());
		GetEffectsComponent()->SetAbilitySystemComponentLink(AbilitySystemComponent);
		
		// ABILITIES
		AbilitySystemComponent->GrantAbilities(PS->GetAbilities()); 
		GetAbilitySystem()->OnStateRequestDelegate.BindUObject(this,&ABaseCharacter::SetState);
		AbilitySystemComponent->OnAbilityCastDelegate.AddDynamic(this, &AMainCharacter::ApplyAbilityVisuals);
		AbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &AMainCharacter::CleanupAbilityVisuals);
		
		AbilitySystemComponent->OnBeginCastDelegate.AddDynamic(GetCharacterMoverComponent(), &UCharacterMoverComponent::ToggleMovement);
		AbilitySystemComponent->OnEndCastDelegate.AddDynamic(GetCharacterMoverComponent(), &UCharacterMoverComponent::ToggleMovement);
		
		AbilitySystemComponent->OnAbilityCastDelegate.AddDynamic(this, &AMainCharacter::AbilityCastDebugSnapshot);
		AbilitySystemComponent->OnAbilityCastFailDelegate.AddDynamic(this, &AMainCharacter::AbilityCastFailDebugSnapshot);
	}
}


// ABILITY VISUALS HANDLING
void AMainCharacter::ApplyAbilityVisuals(UAbilityData* Ability,  FGuid InstanceID,  FAbilityTargetData& Targets)
{
	if (!Ability)
		return;
	GetCharacterAnimationComponent()->PlayAnimation(Ability->AnimationData);
	if (!GetCharacterMoverComponent()->IsCharacterMoving())
	{
		
		GetCharacterAnimationComponent()->RequestRotate(Targets.Direction);
	}
	
	for (FVFXData VFX : Ability->VFXData)
	{
		GetCharacterVFXComponent()->PlayVFX(VFX, InstanceID);
	}
}

void AMainCharacter::CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier)
{
	CleanupVisuals(Identifier);
}


// RESOURCE INTERFACE
float AMainCharacter::GetEnergy_Implementation() const
{
	return ResourceComponent->GetEnergy();
}
void AMainCharacter::ModifyEnergy_Implementation(float Delta)
{
	ResourceComponent->UpdateEnergy(Delta);
}
float AMainCharacter::GetHealth_Implementation() const
{
	return ResourceComponent->GetHealth();
}
void AMainCharacter::ModifyHealth_Implementation(float Delta)
{
	ResourceComponent->UpdateHealth(Delta);
}

void AMainCharacter::TakeDamage_Implementation(float Delta)
{
	if (!bIsAlive) return;

	float HealthLeft = ResourceComponent->GetHealth();
	if (HealthLeft <= 0)
		Die();
	else
		CharacterAnimationComponent->PlayAnimationByTag(FAnimationTags::Animation_CharacterHit);
}

void AMainCharacter::ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effect, FGuid SourceInstanceID)
{
	GetEffectsComponent()->ApplyEffect(EffectOrigin, Effect, SourceInstanceID);
}




// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



// ABILITIES HANDLING
void AMainCharacter::TryUseAbility(FGameplayTag SlotTag)
{
	if (UAbilityData* Ability = GetPlayerState<AMainPlayerState>()->GetAbilityBySlot(SlotTag))
	{
		AbilitySystemComponent->TryUseAbility(Ability);
	}
}
void AMainCharacter::ResolveAbilityInput(FGameplayTag SlotTag,EAbilityInputEvent Event)
{
	UAbilityData* Ability = GetPlayerState<AMainPlayerState>()->GetAbilityBySlot(SlotTag);
	if (!Ability) return;
	
	switch (Event)
	{
	case  EAbilityInputEvent::Completed:
		{
			AbilitySystemComponent->TryAbortAbility(Ability);			
			break;
		}
	case  EAbilityInputEvent::Started:
		{
			AbilitySystemComponent->TryUseAbility(Ability);
			break;
		}
	}
}
UAbilitySystemComponent* AMainCharacter::GetAbilitySystem()
{
	return AbilitySystemComponent;
}
UAbilitySystemComponent* AMainCharacter::GetAbilitySystemComponent_Implementation()
{
	return GetAbilitySystem();
}


void AMainCharacter::OnRespondToHealthChange(float Delta)
{
	Super::OnRespondToHealthChange(Delta);
	if (Delta <= 0)
		TakeDamage_Implementation(Delta);
	
	if (!bIsAlive && GetHealth_Implementation() == 100.f)
	{
		Resurrect();
	}
}


// MESH HANDLING
void AMainCharacter::SetSkeletalDefaults(USkeletalMeshComponent* MeshComponent)
{
	MeshComponent->SetLeaderPoseComponent(GetMesh());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetGenerateOverlapEvents(false);
}
void AMainCharacter::EquipMesh(USkeletalMesh* MeshToEquip, EEquipmentType Type)
{
	switch (Type)
	{
		case EEquipmentType::Hands:
			HandsMesh->SetSkeletalMesh(MeshToEquip);
			break;
		case EEquipmentType::Chest:
			ChestMesh->SetSkeletalMesh(MeshToEquip);
			break;
		case EEquipmentType::Feet:
			FeetMesh->SetSkeletalMesh(MeshToEquip);
			break;
		case EEquipmentType::Head:
			HeadMesh->SetSkeletalMesh(MeshToEquip);
			break;
	}
}
void AMainCharacter::UnEquipMesh(EEquipmentType Type)
{
	switch (Type)
	{
	case EEquipmentType::Hands:
		HandsMesh->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::Chest:
		ChestMesh->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::Feet:
		FeetMesh->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::Head:
		HeadMesh->SetSkeletalMesh(nullptr);
		break;
	}
}




void AMainCharacter::ApplyAttributes()
{
	GetCharacterMoverComponent()->AttributeSpeedMultiplier = 1.0f;
	
	float SpeedAttributeValue = GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Speed);
	float NormalizedSpeed = SpeedAttributeValue / GetAttributesComponent()->MaxSpeedAttribute;
	GetCharacterMoverComponent()->AttributeSpeedMultiplier = 1.0f + NormalizedSpeed; 
	
	GetCharacterMoverComponent()->UpdateSpeed();
}


void AMainCharacter::AbilityCastDebugSnapshot(UAbilityData* Ability, FGuid InstanceID, FAbilityTargetData& Targets)
{
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
void AMainCharacter::AbilityCastFailDebugSnapshot(UAbilityData* Ability, EAbilityFailureReason Reason)
{
	const FText ReasonText =
		StaticEnum<EAbilityFailureReason>()
			->GetDisplayNameTextByValue(
				static_cast<int64>(Reason)
			);

	OnAbilityCastFailedDebugSnapshotDelegate.Broadcast(Reason,Ability->Tag);
}





