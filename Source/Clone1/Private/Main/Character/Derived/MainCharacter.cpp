// Fill out your copyright notice in the Description page of Project Settings.

#include "Main/Character/Derived/MainCharacter.h"
#include "AbilityDatabaseSubsystem.h"
#include "Main/Character/CharacterVFXComponent.h"
#include "Main/Character/CharacterMoverComponent.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "Equipment/EquipmentComponent.h"
#include "Main/Character/AttributesComponent.h"
#include "Main/PlayerController/MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Main/MyGameInstance.h"
#include "SaveSystem/GameSaveSubsystem.h"
#include "Structs/FAnimationTags.h"
#include "Main/Character/InteractorComponent.h"
#include "WeaponSystem/WeaponSystemComponent.h"
#include "Main/Character/CharacterDebugComponent.h"

//                          ░██                          
//                          ░██                          
//  ░███████   ░███████  ░████████ ░██    ░██ ░████████  
// ░██        ░██    ░██    ░██    ░██    ░██ ░██    ░██ 
//  ░███████  ░█████████    ░██    ░██    ░██ ░██    ░██ 
//        ░██ ░██           ░██    ░██   ░███ ░███   ░██ 
//  ░███████   ░███████      ░████  ░█████░██ ░██░█████  
//                                            ░██        
//                                            ░██        
//                                                       

AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	
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

// Character appears in a world
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetEffectsComponent()->SetAttributesComponentLink(GetAttributesComponent());
	GetAttributesComponent()->OnAttributesChangedDelegate.AddDynamic(this, &AMainCharacter::ApplyAttributes);
	
	
	// INPUT
	CharacterInputComponent->OnAbilityInputDelegate.AddDynamic(this, &AMainCharacter::OnAbilityInput);
	CharacterInputComponent->OnInteractDelegate.AddDynamic(this, &AMainCharacter::OnInteractInput);
	
	// Initialize Interaction Component
	InteractionComponent->Init(this);
	
	
	// ATTACH SOME MESHES
	WeaponMesh->AttachToComponent(
		GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);
	
	AbilityProjectileSpawnPoint->AttachToComponent(
		GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);
	
	
	// APPLY LOAD GAME
	if (UGameSaveSubsystem* GameSaveSubsystem = GetGameInstance()->GetSubsystem<UGameSaveSubsystem>())
	{
		GameSaveSubsystem->ApplyPendingLoad(this);
	}
	
}

// State arrives to Owner/Server
void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupFromPlayerState(); // Server
}

// State Arrives to Clients
void AMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	SetupFromPlayerState(); // Client
}

// Request Ability from Server
void AMainCharacter::Server_AbilityInput_Implementation(FGameplayTag SlotTag, EAbilityInputEvent Event,FAbilityTargetData ClientTargetData)
{
	if (!AbilitySystemComponent.IsValid()) return;
	
	AbilitySystemComponent->ResolveAbilityInput_Server(SlotTag, Event, ClientTargetData);
}


void AMainCharacter::SetupFromPlayerState()
{
	if (AMainPlayerState* PS = GetPlayerState<AMainPlayerState>())
	{
		if (PS->InventoryComponent)
		{
			PS->InventoryComponent->OnWeightChangeDelegate.RemoveAll(GetMoverComponent());
			PS->EquipmentComponent->OnItemEquippedDelegate.RemoveAll(this);
			PS->EquipmentComponent->OnItemUnequippedDelegate.RemoveAll(this);
			
			PS->InventoryComponent->OnWeightChangeDelegate.AddDynamic(GetMoverComponent(), &UCharacterMoverComponent::ModifySpeedByWeight);
			PS->EquipmentComponent->OnItemEquippedDelegate.AddDynamic(this, &AMainCharacter::EquipMesh);
			PS->EquipmentComponent->OnItemUnequippedDelegate.AddDynamic(this,  &AMainCharacter::UnEquipMesh);
		}
		
		AbilitySystemComponent = PS->AbilitySystemComponent;
		// Important - set current avatar for the ability system
		AbilitySystemComponent->SetPawn(this);
		
		AbilitySystemComponent->OnAbilityCastedDelegate.RemoveAll(this);
		AbilitySystemComponent->OnAbilityAbortedDelegate.RemoveAll(this);
		AbilitySystemComponent->OnCastStateDelegate.RemoveAll(this);
		
		
		
		// SET COMPONENT LINKS
		GetEffectsComponent()->SetInventoryComponentLink(PS->InventoryComponent);
		GetAttributesComponent()->SetEquipmentComponentLink(PS->EquipmentComponent);
		GetEffectsComponent()->SetAbilitySystemComponentLink(PS->AbilitySystemComponent);
		
		// Visuals bind for everyone
		PS->AbilitySystemComponent->OnAbilityCastedDelegate.AddDynamic(this, &AMainCharacter::ApplyAbilityVisuals);
		PS->AbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &AMainCharacter::CleanupAbilityVisuals);
		PS->AbilitySystemComponent->OnCastStateDelegate.AddDynamic(this, &AMainCharacter::OnAbilityCast);

		// Ability Debug bind for everyone 
		PS->AbilitySystemComponent->OnAbilityCastedDelegate.AddDynamic(GetDebugComponent(), &UCharacterDebugComponent::AbilityCastDebugSnapshot);
		PS->AbilitySystemComponent->OnAbilityCastFailDelegate.AddDynamic(GetDebugComponent(), &UCharacterDebugComponent::AbilityCastFailDebugSnapshot);
		PS->AbilitySystemComponent->OnStateRequestDelegate.BindUObject(this, &ABaseCharacter::SetState);
	
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

//            ░██                                 ░██            
//                                                ░██            
// ░██    ░██ ░██ ░███████  ░██    ░██  ░██████   ░██  ░███████  
// ░██    ░██ ░██░██        ░██    ░██       ░██  ░██ ░██        
//  ░██  ░██  ░██ ░███████  ░██    ░██  ░███████  ░██  ░███████  
//   ░██░██   ░██       ░██ ░██   ░███ ░██   ░██  ░██        ░██ 
//    ░███    ░██ ░███████   ░█████░██  ░█████░██ ░██  ░███████  
//                                                               
                                                              
                                                              
void AMainCharacter::ApplyAbilityVisuals(FGameplayTag AbilityTag,  FGuid InstanceID,  FVector AbilityDirection)
{
	
	UAbilityDatabaseSubsystem* AbilityDatabaseSubsystem = GetGameInstance<UMyGameInstance>()->GetSubsystem<UAbilityDatabaseSubsystem>();
	if (!AbilityDatabaseSubsystem) return;

	UAbilityData* AbilityData = AbilityDatabaseSubsystem->GetAbilityByTag(AbilityTag);
	if (!AbilityData) return;
	
	
	GetAnimationComponent()->PlayAnimation(AbilityData->AnimationData);
	if (!GetMoverComponent()->IsCharacterMoving())
	{
		GetAnimationComponent()->RequestRotate(AbilityDirection);
	}
	
	for (FVFXData VFX : AbilityData->VFXData)
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


//
//
// ░██           ░██                            ░████                                             
// 				 ░██                           ░██                                                
// ░██░████████  ░████████  ░███████  ░██░████ ░████████  ░██████    ░███████   ░███████   ░███████  
// ░██░██    ░██    ░██    ░██    ░██ ░███        ░██          ░██  ░██    ░██ ░██    ░██ ░██        
// ░██░██    ░██    ░██    ░█████████ ░██         ░██     ░███████  ░██        ░█████████  ░███████  
// ░██░██    ░██    ░██    ░██        ░██         ░██    ░██   ░██  ░██    ░██ ░██               ░██ 
// ░██░██    ░██     ░████  ░███████  ░██         ░██     ░█████░██  ░███████   ░███████   ░███████  
//                                                                                                   
//                                                                                                   
//                    

// RESOURCE INTERFACE - It, among energy, serves also as a "Take Damage" functionality.
void AMainCharacter::ApplyResourceDelta_Implementation(ECharacterResource Type, float Delta)
{
	if (!HasAuthority()) return;
	
	switch (Type)
	{
		case ECharacterResource::Health:
			GetResourceComponent()->UpdateHealth(Delta);
			break;
		case ECharacterResource::Energy:
			GetResourceComponent()->UpdateEnergy(Delta);
			break;
		default:
			break;
	}
}

float AMainCharacter::GetEnergy_Implementation() const
{
	return ResourceComponent->GetEnergy();
}

float AMainCharacter::GetHealth_Implementation() const
{
	return ResourceComponent->GetHealth();
}




// CHARACTER EFFECT RECIEVER INTERFACE
void AMainCharacter::ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effect, FGuid SourceInstanceID)
{
	if (!HasAuthority()) return;
	
	GetEffectsComponent()->ApplyEffect(EffectOrigin, Effect, SourceInstanceID);
}


//                                       ░██        
//                                       ░██        
// ░█████████████   ░███████   ░███████  ░████████  
// ░██   ░██   ░██ ░██    ░██ ░██        ░██    ░██ 
// ░██   ░██   ░██ ░█████████  ░███████  ░██    ░██ 
// ░██   ░██   ░██ ░██               ░██ ░██    ░██ 
// ░██   ░██   ░██  ░███████   ░███████  ░██    ░██ 



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


//                                                                                                                     
// ░██                                    ░██                                                                                                   
//                                        ░██                                                                                                   
// ░██░████████  ░████████  ░██    ░██ ░████████          ░██░████  ░███████   ░███████  ░████████   ░███████  ░████████   ░███████   ░███████  
// ░██░██    ░██ ░██    ░██ ░██    ░██    ░██             ░███     ░██    ░██ ░██        ░██    ░██ ░██    ░██ ░██    ░██ ░██        ░██    ░██ 
// ░██░██    ░██ ░██    ░██ ░██    ░██    ░██             ░██      ░█████████  ░███████  ░██    ░██ ░██    ░██ ░██    ░██  ░███████  ░█████████ 
// ░██░██    ░██ ░███   ░██ ░██   ░███    ░██             ░██      ░██               ░██ ░███   ░██ ░██    ░██ ░██    ░██        ░██ ░██        
// ░██░██    ░██ ░██░█████   ░█████░██     ░████          ░██       ░███████   ░███████  ░██░█████   ░███████  ░██    ░██  ░███████   ░███████  
//               ░██                                                                     ░██                                                    
//               ░██                                                                     ░██                                                    
//                                                                                                                                                                                                                                                                                      


void AMainCharacter::OnAbilityInput(FGameplayTag SlotTag, EAbilityInputEvent Event)
{
	UAbilityData* Ability = GetPlayerState<AMainPlayerState>()->GetAbilityBySlot(SlotTag);
	if (!Ability) return;
	
	
	
	if (!IsLocallyControlled())
		return;
	
	if (HasAuthority())
	{
		AbilitySystemComponent->ResolveAbilityInput(Ability->Tag, Event);
	}
	else
	{
		// resolve targeting locally, send that data to server
		FAbilityTargetData OutTargetData;
		AbilitySystemComponent->ResolveTargetingLocally(Ability,OutTargetData);
		
		Server_AbilityInput(Ability->Tag,Event,OutTargetData);
	}
}

void AMainCharacter::OnInteractInput()
{
	InteractionComponent->Interact();
}


void AMainCharacter::OnWeaponInput(EWeaponInputCommand Command)
{
}


//
//                          ░██       ░██                                   
//                          ░██       ░██                                   
//  ░████████  ░███████  ░████████ ░████████  ░███████  ░██░████  ░███████  
// ░██    ░██ ░██    ░██    ░██       ░██    ░██    ░██ ░███     ░██        
// ░██    ░██ ░█████████    ░██       ░██    ░█████████ ░██       ░███████  
// ░██   ░███ ░██           ░██       ░██    ░██        ░██             ░██ 
//  ░█████░██ ░███████      ░████     ░████  ░███████   ░██       ░███████  
//        ░██                                                               
//  ░███████                                                                
//                                                                          



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
	if (AbilitySystemComponent.IsValid())
		return AbilitySystemComponent.Get();
	
	return nullptr;
}


//					░██                      
//                                          
// ░█████████████  ░██ ░███████   ░███████  
// ░██   ░██   ░██ ░██░██        ░██    ░██ 
// ░██   ░██   ░██ ░██ ░███████  ░██        
// ░██   ░██   ░██ ░██       ░██ ░██    ░██ 
// ░██   ░██   ░██ ░██ ░███████   ░███████  
                                         

void AMainCharacter::ApplyAttributes()
{
	GetMoverComponent()->AttributeSpeedMultiplier = 1.0f;
	
	float SpeedAttributeValue = GetAttributesComponent()->GetFinalAttributeValue(EAttribute::Speed);
	float NormalizedSpeed = SpeedAttributeValue / GetAttributesComponent()->MaxSpeedAttribute;
	GetMoverComponent()->AttributeSpeedMultiplier = 1.0f + NormalizedSpeed; 
	
	GetMoverComponent()->UpdateSpeed();
}

void AMainCharacter::OnRespondToHealthChange(float Delta)
{
	if (Delta > 0.0f)
	{
		if (!bIsAlive && GetHealth_Implementation() == 100.f)
		{
			Resurrect();
			return;
		}
	}
	else if (Delta < 0.0f)
	{
		if (!bIsAlive) return;
		float HealthLeft = ResourceComponent->GetHealth();
		if (HealthLeft <= 0)
			Die();
		else
			AnimationComponent->PlayAnimationByTag(FAnimationTags::Animation_CharacterHit);
	}
}


void AMainCharacter::OnAbilityCast(bool bState, FGameplayTag AbilityTag)
{
	UAbilityDatabaseSubsystem* AbilityDatabaseSubsystem = GetGameInstance<UMyGameInstance>()->GetSubsystem<UAbilityDatabaseSubsystem>();
	if (!AbilityDatabaseSubsystem) return;

	UAbilityData* AbilityData = AbilityDatabaseSubsystem->GetAbilityByTag(AbilityTag);
	if (!AbilityData) return;
	
	if (AbilityData->AnimationData.bBlocksMovement)
	{
		MoverComponent->ToggleMovement(!bState);
	}
	
	//
}
