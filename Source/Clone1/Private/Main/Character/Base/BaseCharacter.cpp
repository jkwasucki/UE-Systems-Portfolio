// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/Base/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Main/Character/EffectsComponent.h"
#include "Main/Character/CharacterDebugComponent.h"
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
	MoverComponent = CreateDefaultSubobject<UCharacterMoverComponent>("MovementHandlerComponent");
	VFXComponent = CreateDefaultSubobject<UCharacterVFXComponent>("VFXComponent");
	AnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>("AnimationComponent");
	AnimationComponent->SetCharacterMovementComponentLink(MoverComponent);
	DebugComponent = CreateDefaultSubobject<UCharacterDebugComponent>("DebugComponent");
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AttributesComponent->SetEffectsComponentLink(EffectsComponent);
	
	// Track health delta change
	ResourceComponent->OnHealthChangedByDeltaDelegate.AddDynamic(this,&ABaseCharacter::OnRespondToHealthChange);
	
	EffectsComponent->OnRequestVFXDelegate.AddDynamic(VFXComponent, &UCharacterVFXComponent::PlayVFX);
	EffectsComponent->OnRequestVFXEndDelegate.AddDynamic(VFXComponent, &UCharacterVFXComponent::StopVFX);
	
	MoverComponent->OnStateRequestDelegate.BindUObject(this,&ABaseCharacter::SetState);
	
	// Initialize Debug Component
	DebugComponent->Init(this);
}

void ABaseCharacter::CleanupVisuals(const FGuid& Identifier)
{
	GetAnimationComponent()->StopCurrentMontage();
	GetVFXComponent()->StopVFX(Identifier);
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
	AnimationComponent->PlayAnimationByTag(
		FAnimationTags::Animation_CharacterResurrect
	);
}


// GETTERS =============================================================
//======================================================================
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
UCharacterDebugComponent* ABaseCharacter::GetDebugComponent()
{
	return DebugComponent;
}
UCharacterAnimationComponent* ABaseCharacter::GetAnimationComponent()
{
	return AnimationComponent;
}
UCharacterVFXComponent* ABaseCharacter::GetVFXComponent()
{
	return VFXComponent;
}
UCharacterMoverComponent* ABaseCharacter::GetMoverComponent()
{
	return MoverComponent;	
}
EEntityState ABaseCharacter::GetState()
{
	return  EntityState;
}
FEntityGameplayDebugSnapshot ABaseCharacter::GetDebugInfo_Implementation()
{
	return DebugComponent->RequestEntitySnapshotData();
}
UCharacterAnimationComponent* ABaseCharacter::BP_GetCharacterAnimationComponent() const
{
	return AnimationComponent;
}

void ABaseCharacter::OnRespondToHealthChange(float Delta)
{
	// Override by derived classes
}

bool ABaseCharacter::IsAlive()
{
	return bIsAlive;
}

bool ABaseCharacter::IsEnemy()
{
	return EntityType == EEntityType::Enemy;
}

bool ABaseCharacter::IsAlly()
{
	return EntityType == EEntityType::Ally;
}

void ABaseCharacter::SetState(EEntityState State)
{
	EntityState = State;
	OnStateChangeDelegate.Broadcast(EntityState);
}
