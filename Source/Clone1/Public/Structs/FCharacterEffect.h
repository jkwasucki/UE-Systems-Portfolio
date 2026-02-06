#pragma once
#include "CoreMinimal.h"
#include "Types/AttributeTypes.h"
#include "Types/EffectTypes.h"
#include "Types/CharacterTypes.h"
#include "Structs/FVFXData.h"
#include "FCharacterEffect.generated.h"

USTRUCT(BlueprintType)
struct FCharacterEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TypeID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRevertOnFailure;								// When cast fails and the ability applies effect overtime, changes are reverted on failure
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectSource Source;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttribute TargetedAttribute = EAttribute::None;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECharacterResource TargetedResource = ECharacterResource::None ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectType EffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectMode EffectMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RawValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool  bHasVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVFXData VFXData;
};
