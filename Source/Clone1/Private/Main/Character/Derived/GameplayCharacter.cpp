// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/Derived/GameplayCharacter.h"
#include "Main/Character/EffectsComponent.h"
#include "Structs/FAnimationTags.h"


AGameplayCharacter::AGameplayCharacter()
{

}

void AGameplayCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


float AGameplayCharacter::GetEnergy_Implementation() const
{
	return ResourceComponent->GetEnergy();
}

void AGameplayCharacter::ModifyEnergy_Implementation(float Delta)
{
	if (!bIsAlive) return;
	
	ResourceComponent->UpdateEnergy(Delta);
}


float AGameplayCharacter::GetHealth_Implementation() const
{
	return ResourceComponent->GetHealth();
}
void AGameplayCharacter::ModifyHealth_Implementation(float Delta)
{
	if (!bIsAlive) return;
	
	ResourceComponent->UpdateHealth(Delta);
}

void AGameplayCharacter::TakeDamage_Implementation(float Delta)
{
	if (!bIsAlive) return;

	float HealthLeft = ResourceComponent->GetHealth();
	if (HealthLeft <= 0)
		Die();
	else
		CharacterAnimationComponent->PlayAnimationByTag(FAnimationTags::Animation_CharacterHit);
}

void AGameplayCharacter::ApplyEffect_Implementation(AActor* EffectOrigin, FCharacterEffect& Effect, FGuid SourceInstanceID)
{
	GetEffectsComponent()->ApplyEffect(EffectOrigin, Effect, SourceInstanceID);
}

void AGameplayCharacter::OnRespondToHealthChange(float Delta)
{
	Super::OnRespondToHealthChange(Delta);
	if (Delta <= 0)
		TakeDamage_Implementation(Delta);
	
	if (!bIsAlive && GetHealth_Implementation() == 100.f)
	{
		Resurrect();
	}
}
