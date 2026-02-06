// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Targeting/AOEManualTargetingStrategy.h"

#include "Main/PlayerController/MainPlayerController.h"

void UAoeManualTargetingStrategy::TickTargeting_Implementation(float DeltaTime)
{
	Super::TickTargeting_Implementation(DeltaTime);

	if (!bIsTargeting || !Indicator || !PC)
		return;

	const FVector InstigatorLocation = PC->GetCharacterLocation();
	constexpr float MaxRange = 600.f;
	constexpr float AimTraceLength = 5000.f;
	constexpr float GroundTraceHeight = 5000.f;

	// --------------------------------
	// Camera
	// --------------------------------
	FVector CameraLoc;
	FRotator CameraRot;
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	const FVector AimStart = CameraLoc;
	const FVector AimEnd   = AimStart + CameraRot.Vector() * AimTraceLength;

	// --------------------------------
	// 1) Aim trace: MULTI, then choose first NON-PAWN hit
	// --------------------------------
	TArray<FHitResult> AimHits;

	FCollisionQueryParams AimParams(SCENE_QUERY_STAT(AOE_Aim), false);
	AimParams.AddIgnoredActor(PC->GetPawn());
	AimParams.bTraceComplex = false;
	
	GetWorld()->LineTraceMultiByChannel(
		AimHits,
		AimStart,
		AimEnd,
		ECC_Visibility,
		AimParams
	);

	FVector DesiredLocation = InstigatorLocation + (CameraRot.Vector().GetSafeNormal2D() * MaxRange);

	bool bFoundNonPawnAimHit = false;
	FHitResult BestAimHit;

	for (const FHitResult& H : AimHits)
	{
		if (!H.bBlockingHit)
			continue;

		const UPrimitiveComponent* Comp = H.GetComponent();
		if (!Comp)
			continue;

		// Skip pawns (capsules/meshes)
		if (Comp->GetCollisionObjectType() == ECC_Pawn)
			continue;

		// Dont target walls
		if (H.ImpactNormal.Z < 0.2f)
			continue;

		BestAimHit = H;
		bFoundNonPawnAimHit = true;
		break;
	}

	// --------------------------------
	// 2) Compute desired distance (clamped) from that non-pawn hit if any
	// --------------------------------
	float DesiredDistance = MaxRange;

	if (bFoundNonPawnAimHit)
	{
		FVector FlatDelta = BestAimHit.ImpactPoint - InstigatorLocation;
		FlatDelta.Z = 0.f;
		DesiredDistance = FMath::Clamp(FlatDelta.Size(), 0.f, MaxRange);
	}

	// Flat direction (yaw only)
	FRotator FlatRot = CameraRot;
	FlatRot.Pitch = 0.f;
	FlatRot.Roll  = 0.f;

	const FVector FlatDir = FlatRot.Vector().GetSafeNormal();
	DesiredLocation = InstigatorLocation + FlatDir * DesiredDistance;

	FVector FinalLocation =
		InstigatorLocation + FlatDir * DesiredDistance;

	FHitResult GroundHit;

	const FVector GroundStart =
		FinalLocation + FVector(0.f, 0.f, GroundTraceHeight);

	const FVector GroundEnd =
		FinalLocation - FVector(0.f, 0.f, GroundTraceHeight * 2.f);

	FCollisionObjectQueryParams GroundObjects;
	GroundObjects.AddObjectTypesToQuery(ECC_WorldStatic);
	GroundObjects.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionQueryParams GroundParams;
	GroundParams.bTraceComplex = false;

	if (GetWorld()->LineTraceSingleByObjectType(
		GroundHit,
		GroundStart,
		GroundEnd,
		GroundObjects,
		GroundParams))
	{
		FinalLocation.Z = GroundHit.ImpactPoint.Z;
	}

	// Small lift to avoid z-fighting
	FinalLocation.Z += 1.f;

	// APPLY THE CORRECT LOCATION
	CachedDesiredLocation = FinalLocation;
	Indicator->SetActorLocation(FinalLocation);
}



void UAoeManualTargetingStrategy::AbortTargeting_Implementation()
{
	EndTargeting();
}


EAbilityTargetingStatus UAoeManualTargetingStrategy::TryResolveTargeting_Implementation(AActor* Instigator,
                                                                                        FAbilityIndicatorData& IndicatorData, FCustomTargetingRules& CustomTargetingRules, FAbilityTargetData& iOutTargetData)
{
	if (!bIsTargeting)
	{
		AMainPlayerController* PlayerController = Instigator->GetInstigatorController<AMainPlayerController>();
		StartTargeting(IndicatorData, PlayerController);
		return EAbilityTargetingStatus::InProgress;
	}
	
	iOutTargetData = EndTargeting();
	return EAbilityTargetingStatus::Confirmed;
}


void UAoeManualTargetingStrategy::StartTargeting(FAbilityIndicatorData& IndicatorData, AMainPlayerController* inPC)
{
	PC = inPC;
	bIsTargeting = true;
	SpawnIndicator(IndicatorData);
}

FAbilityTargetData UAoeManualTargetingStrategy::EndTargeting()
{
	FAbilityTargetData TargetData;
	TargetData.TargetLocation = CachedDesiredLocation;
	TargetData.bHasValidTarget = true;
	
	PC = nullptr;
	bIsTargeting = false;
	Indicator->SetActorHiddenInGame(true);
	Indicator->Destroy();
	Indicator = nullptr;
	return TargetData;
}

void UAoeManualTargetingStrategy::SpawnIndicator(FAbilityIndicatorData& IndicatorData)
{
	Indicator = GetWorld()->SpawnActor<AAbilityAreaIndicator>(
		IndicatorData.IndicatorClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator
	);
}
