#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Definitions/AbilityEffect.h"
#include "Types/AbilityTypes.h"
#include "FAbilityProjectileData.generated.h"

USTRUCT(BlueprintType)
struct FAbilityProjectileData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EAbilityProjectileType Type;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float Speed;
	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<UAbilityEffect>> InflictedOnHitEffects;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FVFXData ProjectileVFX;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FVFXData ImpactVFX;
};
