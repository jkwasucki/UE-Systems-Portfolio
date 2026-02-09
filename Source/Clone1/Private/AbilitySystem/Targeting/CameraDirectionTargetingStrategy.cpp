#include "G:\UnrealProjects\Clone1\Intermediate\Build\Win64\x64\Clone1Editor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "AbilitySystem/Targeting/CameraDirectionTargetingStrategy.h"


EAbilityTargetingStatus UCameraDirectionTargetingStrategy::TryResolveTargeting_Implementation(
	AActor* Instigator,
	FAbilityIndicatorData& IndicatorData,
	FCustomTargetingRules& CustomTargetingRules,
	FAbilityTargetData& OutTargetData)
{
	FAbilityTargetData Data;

	if (!Instigator)
		return EAbilityTargetingStatus::Invalid;

	APlayerController* PC = Cast<APlayerController>(Instigator->GetInstigatorController());
	if (!PC)
		return EAbilityTargetingStatus::Invalid;

	int32 ViewX, ViewY;
	PC->GetViewportSize(ViewX, ViewY);

	const float ScreenX = ViewX * 0.5f;
	const float ScreenY = ViewY * 0.5f;

	FVector WorldOrigin;
	FVector WorldDirection;
	if (!PC->DeprojectScreenPositionToWorld(
			ScreenX,
			ScreenY,
			WorldOrigin,
			WorldDirection))
	{
		return EAbilityTargetingStatus::Invalid;
	}

	const FVector TraceStart = WorldOrigin;
	const FVector TraceEnd = TraceStart + WorldDirection * 100000.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Instigator);

	bool bHit = Instigator->GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	FVector AimPoint = bHit ? Hit.ImpactPoint : TraceEnd;

	FVector MuzzleLocation = Instigator->GetActorLocation();
	Data.Direction = (AimPoint - MuzzleLocation).GetSafeNormal();
	Data.bHasValidTarget = true;

	// DEBUG
	DrawDebugLine(
		Instigator->GetWorld(),
		MuzzleLocation,
		MuzzleLocation + Data.Direction * 2000.f,
		FColor::Green,
		false,
		2.f
	);

	OutTargetData = Data;
	return EAbilityTargetingStatus::Confirmed;
}

