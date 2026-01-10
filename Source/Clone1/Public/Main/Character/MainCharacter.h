// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main/PlayerState/AttributesComponent.h"
#include "Types/ItemTypes.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class CLONE1_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	UAttributesComponent* AttributesComponent = nullptr;
	

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float BaseWalkSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float WeightStrengthMultiplier = 4.0f;
	
	
	float WeightSpeedMultiplier = 1.0;
	float AttributeSpeedMultiplier = 1.0f;
	
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	USkeletalMeshComponent* HeadMesh = nullptr;

	UPROPERTY(VisibleAnywhere ,Category = "Equipment")
	USkeletalMeshComponent* ChestMesh = nullptr;

	UPROPERTY(VisibleAnywhere,Category = "Equipment")
	USkeletalMeshComponent* LegsMesh = nullptr;

	UPROPERTY(VisibleAnywhere,Category = "Equipment")
	USkeletalMeshComponent* HandsMesh = nullptr;

	UPROPERTY(VisibleAnywhere,Category = "Equipment")
	USkeletalMeshComponent* FeetMesh = nullptr;
	
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void ModifySpeedByWeight(int32 CurrentWeight, int32 MaxWeight);
	

	UFUNCTION()
	void SetSkeletalDefaults(USkeletalMeshComponent* SkeletalMesh);
	
	UFUNCTION()
	void  EquipMesh(USkeletalMesh* MeshToEquip, EEquipmentType Type);
	
	UFUNCTION()
	void UnEquipMesh(EEquipmentType Type);
	
	UFUNCTION()
	void ApplyAttributes();
	
	UFUNCTION()
	void UpdateSpeed();
	
};
