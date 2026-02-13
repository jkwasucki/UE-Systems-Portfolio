// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/Derived/EnemyCharacter.h"
#include "Main/Character/EffectsComponent.h"
#include "Structs/FAnimationTags.h"


AEnemyCharacter::AEnemyCharacter()
{
	EnemyAIComponent = CreateDefaultSubobject<UEnemyAIComponent>("EnemyAIComponent");
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilityProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AbilityProjectileSpawnPoint"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyAIComponent)
		EnemyAIComponent->Initialize(this);
	
	AbilityProjectileSpawnPoint->AttachToComponent(
		GetOwner()->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);
	
	// ABILITIES
	TArray<UAbilityData*> Abilities;
	Abilities.Add(AvailableAbility);
	AbilitySystemComponent->GrantAbilities(Abilities); 
	AbilitySystemComponent->OnStateRequestDelegate.BindUObject(this,&ABaseCharacter::SetState);
	AbilitySystemComponent->OnAbilityCastDelegate.AddDynamic(this, &AEnemyCharacter::ApplyAbilityVisuals);
	AbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &AEnemyCharacter::CleanupAbilityVisuals);
	AbilitySystemComponent->OnBeginCastDelegate.AddDynamic(GetMoverComponent(), &UCharacterMoverComponent::ToggleMovement);
	AbilitySystemComponent->OnEndCastDelegate.AddDynamic(GetMoverComponent(), &UCharacterMoverComponent::ToggleMovement);
}


float AEnemyCharacter::GetEnergy_Implementation() const
{
	return ResourceComponent->GetEnergy();
}

void AEnemyCharacter::ModifyEnergy_Implementation(float Delta)
{
	if (!bIsAlive) return;
	
	ResourceComponent->UpdateEnergy(Delta);
}


float AEnemyCharacter::GetHealth_Implementation() const
{
	return ResourceComponent->GetHealth();
}
void AEnemyCharacter::ModifyHealth_Implementation(float Delta)
{
	if (!bIsAlive) return;
	
	ResourceComponent->UpdateHealth(Delta);
}

void AEnemyCharacter::TakeDamage_Implementation(float Delta)
{
	if (!bIsAlive) return;

	float HealthLeft = ResourceComponent->GetHealth();
	if (HealthLeft <= 0)
		Die();
	else
		AnimationComponent->PlayAnimationByTag(FAnimationTags::Animation_CharacterHit);
}

void AEnemyCharacter::ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effect, FGuid SourceInstanceID)
{
	GetEffectsComponent()->ApplyEffect(EffectOrigin, Effect, SourceInstanceID);
}

void AEnemyCharacter::OnRespondToHealthChange(float Delta)
{
	Super::OnRespondToHealthChange(Delta);
	if (Delta <= 0)
		TakeDamage_Implementation(Delta);
	
	if (!bIsAlive && GetHealth_Implementation() == 100.f)
	{
		Resurrect();
	}
}
// ABILITY VISUALS HANDLING
void AEnemyCharacter::ApplyAbilityVisuals(UAbilityData* Ability,  FGuid InstanceID,  FAbilityTargetData& Targets)
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

void AEnemyCharacter::CleanupAbilityVisuals(AActor* AbilityOwner, const FGuid& Identifier)
{
	CleanupVisuals(Identifier);
}
UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent_Implementation()
{
	return AbilitySystemComponent;
}
