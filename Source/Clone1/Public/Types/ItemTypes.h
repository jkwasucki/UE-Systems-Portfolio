#pragma once
#include "CoreMinimal.h"
#include "ItemTypes.generated.h"


UENUM(BlueprintType)
enum class EItemType: uint8
{
	None		UMETA(DisplayName = "None"),
	Weapon		UMETA(DisplayName = "Weapon"),
	Armor		UMETA(DisplayName = "Armor"),
	Consumable	UMETA(DisplayName = "Consumable"),
};

UENUM(BlueprintType)
enum class EEquipmentType: uint8
{
	None		UMETA(DisplayName = "None"),
	Feet		UMETA(DisplayName = "Feet"),
	Legs		UMETA(DisplayName = "Legs"),
	Hands		UMETA(DisplayName = "Hands"),
	Chest		UMETA(DisplayName = "Chest"),
	Head		UMETA(DisplayName = "Head"),
	MainHand	UMETA(DisplayName = "Main Hand Weapon"),
};

UENUM(BlueprintType)
enum class EConsumableType:uint8
{
	None		UMETA(DisplayName = "None"),
	Potion		UMETA(DisplayName = "Potion"),
};

