// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/Base/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Main/Character/EffectsComponent.h"
#include "Structs/FAnimationTags.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ResourceComponent = CreateDefaultSubobject<UResourceComponent>(FName("ResourceComponent"));
	EffectsComponent = CreateDefaultSubobject<UEffectsComponent>("EffectsComponent");
	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(FName("AttributesComponent"));
	CharacterMoverComponent = CreateDefaultSubobject<UCharacterMoverComponent>("MovementHandlerComponent");
	CharacterVFXComponent = CreateDefaultSubobject<UCharacterVFXComponent>("CharacterVFXComponent");
	CharacterAnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>("CharacterAnimationComponent");
	CharacterAnimationComponent->SetCharacterMovementComponentLink(CharacterMoverComponent);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Track health delta change
	ResourceComponent->OnHealthChangedByDeltaDelegate.AddDynamic(this,&ABaseCharacter::OnRespondToHealthChange);
	EffectsComponent->OnRequestVFXDelegate.AddDynamic(CharacterVFXComponent, &UCharacterVFXComponent::PlayVFX);
	EffectsComponent->OnRequestVFXEndDelegate.AddDynamic(CharacterVFXComponent, &UCharacterVFXComponent::StopVFX);
	
	CharacterMoverComponent->OnStateRequestDelegate.BindUObject(this,&ABaseCharacter::SetState);
	ListenForDebugSnapshots();
	RequestDebugSnapshots();
}
void ABaseCharacter::RequestDebugSnapshots()
{
	FEntityGameplayDebugSnapshot Snapshot = RequestEntitySnapshotData();
	OnDisplayDebugSnapshots_AllDelegate.Broadcast(Snapshot);
}

// DEBUG SNAPSHOTS
void ABaseCharacter::ListenForDebugSnapshots()
{
	GetResourceComponent()->SnapshotOnHealthChangedDelegate.AddDynamic(this, &ABaseCharacter::ResourcesChangeDebugSnapshot);
	GetResourceComponent()->SnapshotOnEnergyChangedDelegate.AddDynamic(this, &ABaseCharacter::ResourcesChangeDebugSnapshot);
	GetAttributesComponent()->OnAttributesChangedDelegate.AddDynamic(this, &ABaseCharacter::AttributesChangeDebugSnapshot);
	GetEffectsComponent()->OnEffectEndDelegate.AddDynamic(this, &ABaseCharacter::EffectExpiredDebugSnapshot);
	GetEffectsComponent()->OnEffectStartDelegate.AddDynamic(this, &ABaseCharacter::EffectAppearedDebugSnapshot);
}

void ABaseCharacter::ResourcesChangeDebugSnapshot()
{
	FResourceDebugSnapshot Snapshot = RequestEntitySnapshotData().ResourceDebugSnapshot;
	OnResourcesDebugSnapshotDelegate.Broadcast(Snapshot);
}
void ABaseCharacter::AttributesChangeDebugSnapshot()
{
	FAttributeDebugSnapshot Snapshot = RequestEntitySnapshotData().AttributeSnapshot;
	OnAttributeDebugSnapshotDelegate.Broadcast(Snapshot);
}
void ABaseCharacter::EffectExpiredDebugSnapshot(UActiveEffectInstance* EffectInstance)
{
	FAbilityDebugSnapshot Snapshot;
	Snapshot.Effects.Add(EffectInstance->CharacterEffectDefinition);
	
	OnEffectExpiredDebugSnapshotDelegate.Broadcast(Snapshot);
}
void ABaseCharacter::EffectAppearedDebugSnapshot(UActiveEffectInstance* EffectInstance)
{
	FAbilityDebugSnapshot Snapshot;
	Snapshot.Effects.Add(EffectInstance->CharacterEffectDefinition);
	
	OnEffectAppearDebugSnapshotDelegate.Broadcast(Snapshot);
}

void ABaseCharacter::CleanupVisuals(const FGuid& Identifier)
{
	GetCharacterAnimationComponent()->StopCurrentMontage();
	GetCharacterVFXComponent()->StopVFX(Identifier);
}

void ABaseCharacter::Die()
{
	if (!bIsAlive) return;

	bIsAlive = false;

	// Stop character movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	// Capsule should NOT interfere
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	USkeletalMeshComponent* MeshComp = GetMesh();

	// Detach mesh from capsule BEFORE physics
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	// Enable ragdoll
	MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->WakeAllRigidBodies();
	
	SetState(EEntityState::Dead);
}

void ABaseCharacter::Resurrect()
{
	if (bIsAlive) return;

	bIsAlive = true;
	SetState(EEntityState::Idle);
	
	USkeletalMeshComponent* MeshComp = GetMesh();

	// Stop ragdoll
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

	// Snap mesh back to capsule
	MeshComp->AttachToComponent(
		GetCapsuleComponent(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale
	);

	// Reset relative transform (IMPORTANT)
	MeshComp->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0.f, -90.f, 0.f)
	);

	// Restore capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Restore movement
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// Force anim graph reset
	MeshComp->ResetAllBodiesSimulatePhysics();

	// Play resurrect animation
	CharacterAnimationComponent->PlayAnimationByTag(
		FAnimationTags::Animation_CharacterResurrect
	);
}


UAttributesComponent* ABaseCharacter::GetAttributesComponent()
{
	return AttributesComponent;
}

UEffectsComponent* ABaseCharacter::GetEffectsComponent()
{
	return EffectsComponent;
}

UResourceComponent* ABaseCharacter::GetResourceComponent()
{
	return ResourceComponent;
}

UCharacterAnimationComponent* ABaseCharacter::GetCharacterAnimationComponent()
{
	return CharacterAnimationComponent;
}

UCharacterVFXComponent* ABaseCharacter::GetCharacterVFXComponent()
{
	return CharacterVFXComponent;
}

UCharacterMoverComponent* ABaseCharacter::GetCharacterMoverComponent()
{
	return CharacterMoverComponent;	
}

EEntityState ABaseCharacter::GetState()
{
	return  EntityState;
}

void ABaseCharacter::OnRespondToHealthChange(float Delta)
{
	// Override by derived classes
}

bool ABaseCharacter::IsAlive()
{
	return bIsAlive;
}

FEntityGameplayDebugSnapshot ABaseCharacter::GetDebugInfo_Implementation()
{
	return RequestEntitySnapshotData();
}

bool ABaseCharacter::IsEnemy()
{
	return EntityType == EEntityType::Enemy;
}

bool ABaseCharacter::IsAlly()
{
	return EntityType == EEntityType::Ally;
}


UCharacterAnimationComponent* ABaseCharacter::BP_GetCharacterAnimationComponent() const
{
	return CharacterAnimationComponent;
}

FEntityGameplayDebugSnapshot ABaseCharacter::RequestEntitySnapshotData() const
{
	FEntityGameplayDebugSnapshot Snapshot;

	// Entity info
	Snapshot.EntityType  = EntityType;
	Snapshot.EntityState = EntityState;

	// Attributes
	Snapshot.AttributeSnapshot.Attack =
		AttributesComponent->GetFinalAttributeValue(EAttribute::Attack);

	Snapshot.AttributeSnapshot.Speed =
		AttributesComponent->GetFinalAttributeValue(EAttribute::Speed);

	Snapshot.AttributeSnapshot.Armor =
		AttributesComponent->GetFinalAttributeValue(EAttribute::Armor);

	Snapshot.AttributeSnapshot.Health =
		AttributesComponent->GetFinalAttributeValue(EAttribute::Health);

	// Resources
	Snapshot.ResourceDebugSnapshot.Health =
		ResourceComponent->GetHealth();

	Snapshot.ResourceDebugSnapshot.Energy =
		ResourceComponent->GetEnergy();
	Snapshot.AbilityDebugSnapshot.Effects = EffectsComponent->GetActiveEffectsDefinitions();
	
	return Snapshot;
}

void ABaseCharacter::SetState(EEntityState State)
{
	EntityState = State;
	OnStateChangeDelegate.Broadcast(EntityState);
}
