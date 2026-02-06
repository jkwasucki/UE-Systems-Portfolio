// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Misc/AbilityAreaOccurence.h"

#include "NiagaraFunctionLibrary.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Interfaces/DamageableInterface.h"

// Sets default values
AAbilityAreaOccurence::AAbilityAreaOccurence()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	
	Collider->SetCollisionProfileName(TEXT("Custom"));
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetCollisionObjectType(ECC_WorldDynamic);
	//Ignore all first
	Collider->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Then set pawn explicitly
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AAbilityAreaOccurence::OnOverlapped);
}
void AAbilityAreaOccurence::InitializeAreaOccurence(AActor* InOriginActor,FAbilityAreaOccurenceData& Data,FVector& SpawnLocation)
{
	OriginActor = InOriginActor;
	AreaOccurenceData = Data;
	NiagaraSystem = Data.AreaVFX.Niagara;
	SpawnNiagara(SpawnLocation);
	
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAbilityAreaOccurence::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == GetOwner()) return;
	
	
	if (!OtherActor->Implements<UCharacterEffectReceiverInterface>())
		return;
	
	
	for (TSubclassOf<UAbilityEffect> EffectClass : AreaOccurenceData.InflictedOnHitEffects)
	{
		if (!EffectClass) continue;

		UAbilityEffect* Effect = NewObject<UAbilityEffect>(this, EffectClass);

		ICharacterEffectReceiverInterface::Execute_ApplyEffect(
			OtherActor,
			OriginActor,
			Effect->EffectData,
			FGuid()
		);
	}
	
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SetLifeSpan(2.f);
} 

void AAbilityAreaOccurence::SpawnNiagara(FVector SpawnLocation)
{
	if (!NiagaraSystem) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
			NiagaraSystem,
			SpawnLocation,
			FRotator::ZeroRotator,
			FVector(1),
			true
	);
}

