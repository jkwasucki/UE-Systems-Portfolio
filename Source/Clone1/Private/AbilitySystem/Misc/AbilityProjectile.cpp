// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Misc/AbilityProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interfaces/CharacterEffectReciverInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAbilityProjectile::AAbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	
	ProjectileMoverComponent = CreateDefaultSubobject<UProjectileMoverComponent>("MoverComponent");
	
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetGenerateOverlapEvents(true);

	Collider->SetCollisionObjectType(ECC_WorldDynamic);

	// Ignore everything first
	Collider->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Only overlap pawns
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Collider->SetMobility(EComponentMobility::Movable);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AAbilityProjectile::OnOverlapped);
	
} 

void AAbilityProjectile::OnRep_Initialized()
{
	InitializeProjectile(RepOriginActor.Get(),RepProjectileData,RepTargetData);
}

void AAbilityProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAbilityProjectile, RepProjectileData);
	DOREPLIFETIME(AAbilityProjectile, RepOriginActor);
	DOREPLIFETIME(AAbilityProjectile, bIsInitialized);
	DOREPLIFETIME(AAbilityProjectile, RepTargetData);
}

void AAbilityProjectile::InitializeProjectile(AActor* InOriginActor,FAbilityProjectileData& Data, FAbilityTargetData& TargetData)
{
	ProjectileData = Data;
	NiagaraSystem = Data.ProjectileVFX.Niagara;
	OriginActor = InOriginActor;
	SpawnNiagara();
	ProjectileMoverComponent->InitializeFromTargetData(TargetData);
	
	
	// Replication
	RepOriginActor = InOriginActor;
	RepProjectileData = Data;
	RepTargetData = TargetData;
	bIsInitialized = true;
	
}

void AAbilityProjectile::OnOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || OtherActor == OriginActor)
	{
		return;
	}
	
	// Ignore AI trigger spheres
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel3)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"), *OtherComp->GetName());
	
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMoverComponent->Stop();
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
		NiagaraComponent->SetAutoDestroy(true);
		NiagaraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	if (OtherActor->Implements<UCharacterEffectReceiverInterface>())
	{
		for (TSubclassOf<UAbilityEffect> EffectClass : ProjectileData.InflictedOnHitEffects)
		{
			if (!EffectClass) continue;

			UAbilityEffect* Effect = NewObject<UAbilityEffect>(this, EffectClass);

			ICharacterEffectReceiverInterface::Execute_ApplyEffect(
				OtherActor,        // target (implements interface)
				OriginActor.Get(),       // source
				Effect->EffectData,
				FGuid()
			);
		}
	}
	
	SpawnHitNiagara();
	SetLifeSpan(1);
}

void AAbilityProjectile::SpawnHitNiagara()
{
	if (ProjectileData.ImpactVFX.Niagara == nullptr) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		ProjectileData.ImpactVFX.Niagara,
		GetActorLocation(),
		FRotator::ZeroRotator,
		FVector(1),
		true
	);
}



void AAbilityProjectile::SpawnNiagara()
{
	if (!NiagaraSystem) return;
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,
			Collider,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false
	);
}

