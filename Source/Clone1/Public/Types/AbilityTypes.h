#pragma once
#include "CoreMinimal.h"
#include "AbilityTypes.generated.h"


UENUM(BlueprintType)
enum class EAbilityFailureReason : uint8
{
	None,
	OnCooldown,
	InsufficientEnergy,
	InvalidTarget,
	TargetingInProgress
};

UENUM(BlueprintType)
enum class EAbilityName : uint8
{
	None,
	HealRestoration,
	EnergyRestoriation
};

UENUM(BlueprintType)
enum class EAbilityCastMode : uint8
{
	None,
	Instant,
	Casted,
};
UENUM(BlueprintType)
enum class ETargetingStrategy : uint8
{
	None,
	SelfTargeting,
	LineTraceTargeting,
};
UENUM(BlueprintType)
enum class EAbilityInputEvent : uint8
{
	Triggered,
	Started,
	Canceled,
	Completed,
};
UENUM(BlueprintType)
enum class EAbilityProjectileType : uint8
{
	Free,
	TargetLocked,
};
UENUM(BlueprintType)
enum class EAbilityTargetingStatus : uint8
{
	InProgress,
	Invalid,
	Confirmed,
};