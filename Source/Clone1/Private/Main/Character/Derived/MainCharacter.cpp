// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/Derived/MainCharacter.h"

#include "Main/Character/CharacterVFXComponent.h"
#include "Main/Character/CharacterMoverComponent.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "Equipment/EquipmentComponent.h"
#include "Main/Character/AttributesComponent.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "SaveSystem/GameSaveSubsystem.h"
#include "Structs/FAnimationTags.h"
#include "Main/Character/InteractorComponent.h"
#include "WeaponSystem/WeaponSystemComponent.h"
#include "Main/Character/CharacterDebugComponent.h"


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

	WeaponSystemComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponSystemComponent");
	WeaponSystemComponent->SetComponentTickEnabled(true);
	
	CharacterInputComponent = CreateDefaultSubobject<UCharacterInputComponent>(TEXT("InputComponent"));
	
	InteractionComponent = CreateDefaultSubobject<UInteractorComponent>("InteractionComponent");
	InteractionComponent->SetComponentTickEnabled(true);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	
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
		if (PS->InventoryComponent)
		{
			PS->InventoryComponent->OnWeightChangeDelegate.AddDynamic(GetMoverComponent(), &UCharacterMoverComponent::ModifySpeedByWeight);
			PS->EquipmentComponent->OnItemEquippedDelegate.AddDynamic(this, &AMainCharacter::EquipMesh);
			PS->EquipmentComponent->OnItemUnequippedDelegate.AddDynamic(this,  &AMainCharacter::UnEquipMesh);
		}
		
		// SET COMPONENT LINKS
		GetEffectsComponent()->SetInventoryComponentLink(PS->InventoryComponent);
		GetEffectsComponent()->SetAttributesComponentLink(GetAttributesComponent());
		GetEffectsComponent()->SetAbilitySystemComponentLink(AbilitySystemComponent);
		GetAttributesComponent()->SetEquipmentComponentLink(PS->EquipmentComponent);
		
		// ABILITIES
		AbilitySystemComponent->GrantAbilities(PS->GetAbilities()); 
		GetAbilitySystem()->OnStateRequestDelegate.BindUObject(this,&ABaseCharacter::SetState);
		AbilitySystemComponent->OnAbilityCastDelegate.AddDynamic(this, &AMainCharacter::ApplyAbilityVisuals);
		AbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &AMainCharacter::CleanupAbilityVisuals);
		AbilitySystemComponent->OnBeginCastDelegate.AddDynamic(GetMoverComponent(), &UCharacterMoverComponent::ToggleMovement);
		AbilitySystemComponent->OnEndCastDelegate.AddDynamic(GetMoverComponent(), &UCharacterMoverComponent::ToggleMovement);
		AbilitySystemComponent->OnAbilityCastDelegate.AddDynamic(GetDebugComponent(), &UCharacterDebugComponent::AbilityCastDebugSnapshot);
		AbilitySystemComponent->OnAbilityCastFailDelegate.AddDynamic(GetDebugComponent(), &UCharacterDebugComponent::AbilityCastFailDebugSnapshot);
		
	}
	
	GetAttributesComponent()->OnAttributesChangedDelegate.AddDynamic(this, &AMainCharacter::ApplyAttributes);
	// INPUT
	CharacterInputComponent->OnAbilityInputDelegate.AddDynamic(this, &AMainCharacter::OnAbilityInput);
	CharacterInputComponent->OnInteractDelegate.AddDynamic(this, &AMainCharacter::OnInteractInput);
	// Initialize Interaction Component
	InteractionComponent->Init(this);
	
	
	WeaponMesh->AttachToComponent(
		GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);
	
	// APPLY LOAD GAME
	if (UGameSaveSubsystem* GameSaveSubsystem = GetGameInstance()->GetSubsystem<UGameSaveSubsystem>())
	{
		GameSaveSubsystem->ApplyPendingLoad(this);
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EI =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetController());
	// Initialize Input Component
	CharacterInputComponent->Init(PC,EI);
	
}

// ABILITY VISUALS HANDLING
void AMainCharacter::ApplyAbilityVisuals(UAbilityData* Ability,  FGuid InstanceID,  FAbilityTargetData& Targets)
{
	if (!Ability)
		return;
	GetAnimationComponent()->PlayAnimation(Ability->AnimationData);
	if (!GetMoverComponent()->IsCharacterMoving())
	{
		GetAnimationComponent()->RequestRotate(Targets.Direction);
	}
	
	for (FVFXData VFX : Ability->VFXData)
	{
		GetVFXComponent()->PlayVFX(VFX, InstanceID);
	}
}
void AMainCharacter::CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier)
{
	CleanupVisuals(Identifier);
}

// WEAPON VISUALS HANDLING
void AMainCharacter::DisplayWeaponVisuals(UWeaponInstance* WeaponInstance)
{
	if (!IsValid(WeaponInstance)) return;
	UStaticMesh* FoundMesh = WeaponInstance->GetWeaponDefinition()->Mesh.LoadSynchronous();
	if (!FoundMesh) return;
	WeaponMesh->SetStaticMesh(FoundMesh);
}
void AMainCharacter::OnWeaponInput(EWeaponInputCommand Command)
{
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
		AnimationComponent->PlayAnimationByTag(FAnimationTags::Animation_CharacterHit);
}

void AMainCharacter::ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effect, FGuid SourceInstanceID)
{
	GetEffectsComponent()->ApplyEffect(EffectOrigin, Effect, SourceInstanceID);
}





// ABILITIES HANDLING
void AMainCharacter::TryUseAbility(FGameplayTag SlotTag)
{
	if (UAbilityData* Ability = GetPlayerState<AMainPlayerState>()->GetAbilityBySlot(SlotTag))
	{
		AbilitySystemComponent->TryUseAbility(Ability);
	}
}

UAbilitySystemComponent* AMainCharacter::GetAbilitySystem()
{
	return AbilitySystemComponent;
}
UWeaponComponent* AMainCharacter::GetWeaponSystem()
{
	return WeaponSystemComponent;
}
UCharacterInputComponent* AMainCharacter::GetCharacterInput()
{
	return CharacterInputComponent;
}

UInteractorComponent* AMainCharacter::GetInteractionComponent()
{
	return InteractionComponent;
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
	GetMoverComponent()->AttributeSpeedMultiplier = 1.0f;
	
	float SpeedAttributeValue = GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Speed);
	float NormalizedSpeed = SpeedAttributeValue / GetAttributesComponent()->MaxSpeedAttribute;
	GetMoverComponent()->AttributeSpeedMultiplier = 1.0f + NormalizedSpeed; 
	
	GetMoverComponent()->UpdateSpeed();
}

void AMainCharacter::OnAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event)
{
	UAbilityData* Ability = GetPlayerState<AMainPlayerState>()->GetAbilityBySlot(SlotTag);
	if (!Ability) return;
	
	AbilitySystemComponent->ResolveAbilityInput(Ability, Event);
}

void AMainCharacter::OnInteractInput()
{
	InteractionComponent->Interact();
}





