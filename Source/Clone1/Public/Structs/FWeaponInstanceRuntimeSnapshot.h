#pragma once
#include "CoreMinimal.h"
#include "Types/WeaponTypes.h"
#include "FWeaponInstanceRuntimeSnapshot.generated.h"


class UWeaponDefinition;

USTRUCT(BlueprintType)
struct FWeaponInstanceRuntimeSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid InstanceID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponState WeaponState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponFireMode WeaponFireMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOwnerExists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDefinitionExists;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWeaponDefinition> Definition;
};