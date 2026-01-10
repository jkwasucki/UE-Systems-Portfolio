#pragma once


#include "Types/AttributeTypes.h"
#include "Engine/DataTable.h"
#include "AttributeValue.h"
#include "Actors/Item.h"
#include "Types/ItemTypes.h"
#include "FItemData.generated.h"

USTRUCT(BlueprintType)
struct FConsumableEffect
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttribute Attribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	
	float Value;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration;
};

USTRUCT(BlueprintType)
struct FItemConsumableData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EConsumableType ConsumableType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FConsumableEffect ConsumableEffect;
};

USTRUCT(BlueprintType)
struct FItemEquipableData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EEquipmentType EquipmentType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* VisualMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAttribute, FAttributeValue> Modifiers;
};

USTRUCT(BlueprintType)
struct FItemBaseData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName ItemID;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ItemName;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 MaxStack = 1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Weight = 1;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AItem> ItemClass;
};


// BASE ITEM CLASS
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemBaseData ItemBaseData;
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsEquipable;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(EditCondition="bIsEquipable"))
	FItemEquipableData ItemEquipableData;
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsConsumable;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(EditCondition="bIsConsumable"))
	FItemConsumableData ItemConsumableData;
};

