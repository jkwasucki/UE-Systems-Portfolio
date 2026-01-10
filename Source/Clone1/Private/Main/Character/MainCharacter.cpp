// Fill out your copyright notice in the Description page of Project Settings.


#include "Main/Character/MainCharacter.h"
#include "Main/PlayerState/MainPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(GetMesh());

	ChestMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(GetMesh());

	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMesh"));
	LegsMesh->SetupAttachment(GetMesh());

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandsMesh"));
	HandsMesh->SetupAttachment(GetMesh());

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMesh->SetupAttachment(GetMesh());

	SetSkeletalDefaults(HeadMesh);
	SetSkeletalDefaults(ChestMesh);
	SetSkeletalDefaults(LegsMesh);
	SetSkeletalDefaults(HandsMesh);
	SetSkeletalDefaults(FeetMesh);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AMainPlayerState* PS = GetPlayerState<AMainPlayerState>())
	{
		if (PS->InventoryComponent)
		{
			PS->InventoryComponent->OnWeightChangeDelegate.AddDynamic(this, &AMainCharacter::ModifySpeedByWeight);
			PS->EquipmentComponent->OnItemEquippedDelegate.AddDynamic(this, &AMainCharacter::EquipMesh);
			PS->EquipmentComponent->OnItemUnequippedDelegate.AddDynamic(this,  &AMainCharacter::UnEquipMesh);
			PS->AttributesComponent->OnAttributesChangedDelegate.AddDynamic(this, &AMainCharacter::ApplyAttributes);
			AttributesComponent = PS->AttributesComponent;
		}
	}
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::ModifySpeedByWeight(int32 CurrentWeight, int32 MaxWeight)
{
	if (!GetCharacterMovement()) return;
	
	float NewSpeed = BaseWalkSpeed;
	if (CurrentWeight > MaxWeight)
	{
		const float OverRation = float(CurrentWeight - MaxWeight) / float(MaxWeight);
		WeightSpeedMultiplier = FMath::Clamp(1.f - OverRation * WeightStrengthMultiplier, 0.3f, 1.f);
	}
	else
	{
		WeightSpeedMultiplier = 1.f;
	}

	UpdateSpeed();
}


void AMainCharacter::SetSkeletalDefaults(USkeletalMeshComponent* MeshComponent)
{
	MeshComponent->SetLeaderPoseComponent(GetMesh());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetGenerateOverlapEvents(false);
}

void AMainCharacter::EquipMesh(USkeletalMesh* MeshToEquip, EEquipmentType Type)
{
	switch (Type)
	{
		case EEquipmentType::Hands:
			HandsMesh->SetSkeletalMesh(MeshToEquip);
			break;
		case EEquipmentType::Chest:
			ChestMesh->SetSkeletalMesh(MeshToEquip);
			break;
		case EEquipmentType::Feet:
			FeetMesh->SetSkeletalMesh(MeshToEquip);
			break;
		case EEquipmentType::Head:
			HeadMesh->SetSkeletalMesh(MeshToEquip);
			break;
	}
}

void AMainCharacter::UnEquipMesh(EEquipmentType Type)
{
	switch (Type)
	{
	case EEquipmentType::Hands:
		HandsMesh->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::Chest:
		ChestMesh->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::Feet:
		FeetMesh->SetSkeletalMesh(nullptr);
		break;
	case EEquipmentType::Head:
		HeadMesh->SetSkeletalMesh(nullptr);
		break;
	}
}

void AMainCharacter::ApplyAttributes()
{
	float SpeedAttributeValue = AttributesComponent->GetFinalAttributeValue(EAttribute::Speed);
	float NormalizedSpeed = SpeedAttributeValue / AttributesComponent->MaxSpeedAttribute;
	AttributeSpeedMultiplier = 1.0f + NormalizedSpeed; 
	
	UpdateSpeed();
}

void AMainCharacter::UpdateSpeed()
{
	float Speed = WeightSpeedMultiplier * AttributeSpeedMultiplier;
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * Speed;
}





