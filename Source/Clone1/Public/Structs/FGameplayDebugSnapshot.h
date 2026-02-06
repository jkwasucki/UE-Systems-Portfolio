#pragma once
#include "CoreMinimal.h"
#include "AbilitySystem/Definitions/AbilityData.h"
#include "FGameplayDebugSnapshot.generated.h"

USTRUCT(BlueprintType)
struct FResourceDebugSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Energy = 0;
	
};

USTRUCT(BlueprintType)
struct FAttributeDebugSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armor = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0;
};


USTRUCT(BlueprintType)
struct FAbilityDebugSnapshot
{
	GENERATED_BODY()
	
	// ABILITY
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AbilityTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityCastMode AbilityCastMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid  AbilityInstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnergyCost = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLocksMovement = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAnimationBodyPart AnimationMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TargetingStrategy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCharacterEffect> Effects;
};


USTRUCT(BlueprintType)
struct FEntityGameplayDebugSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityType EntityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEntityState EntityState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAttributeDebugSnapshot AttributeSnapshot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FResourceDebugSnapshot ResourceDebugSnapshot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAbilityDebugSnapshot AbilityDebugSnapshot;
};


	
