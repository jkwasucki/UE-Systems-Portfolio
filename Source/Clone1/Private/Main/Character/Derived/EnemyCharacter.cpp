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
	
	AbilitySystemComponent->SetPawn(this);
	
	// ABILITIES
	TArray<UAbilityData*> Abilities;
	Abilities.Add(AvailableAbility);
	AbilitySystemComponent->GrantAbilities(Abilities); 
	AbilitySystemComponent->OnStateRequestDelegate.BindUObject(this,&ABaseCharacter::SetState);
	AbilitySystemComponent->OnAbilityCastedDelegate.AddDynamic(this, &AEnemyCharacter::ApplyAbilityVisuals);
	AbilitySystemComponent->OnAbilityAbortedDelegate.AddDynamic(this, &AEnemyCharacter::CleanupAbilityVisuals);
}


float AEnemyCharacter::GetEnergy_Implementation() const
{
	return ResourceComponent->GetEnergy();
}


float AEnemyCharacter::GetHealth_Implementation() const
{
	return ResourceComponent->GetHealth();
}


void AEnemyCharacter::ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effect, FGuid SourceInstanceID)
{
	GetEffectsComponent()->ApplyEffect(EffectOrigin, Effect, SourceInstanceID);
}

void AEnemyCharacter::OnRespondToHealthChange(float Delta)
{
	Super::OnRespondToHealthChange(Delta);
	
	if (!bIsAlive && GetHealth_Implementation() == 100.f)
		Resurrect();

	if (!bIsAlive) return;

	float HealthLeft = ResourceComponent->GetHealth();
	if (HealthLeft <= 0)
		Die();
	else
		AnimationComponent->PlayAnimationByTag(FAnimationTags::Animation_CharacterHit);
}

void AEnemyCharacter::ApplyResourceDelta_Implementation(ECharacterResource Type, float Delta)
{
	
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

// ABILITY VISUALS HANDLING
void AEnemyCharacter::ApplyAbilityVisuals(FGameplayTag AbilityTag,  FGuid InstanceID,  FVector AbilityDirection)
{
	if (!AvailableAbility)
		return;
	GetAnimationComponent()->PlayAnimation(AvailableAbility->AnimationData);
	if (!GetMoverComponent()->IsCharacterMoving())
	{
		
		GetAnimationComponent()->RequestRotate(AbilityDirection);
	}
	
	for (FVFXData VFX : AvailableAbility->VFXData)
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
void AEnemyCharacter::OnAbilityCast(bool bState)
{
	MoverComponent->ToggleMovement(!bState);
}
