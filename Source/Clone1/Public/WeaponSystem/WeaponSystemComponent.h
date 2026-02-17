// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSystem/Definitions/WeaponDefinition.h"
#include "WeaponSystem/Instances/WeaponInstance.h"
#include "Components/ActorComponent.h"
#include "WeaponSystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponGranted, UWeaponInstance*, WeaponInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponInstanceUpdate, UWeaponInstance*, WeaponInstance);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(VisibleAnywhere)
	UWeaponInstance* CurrentWeapon = nullptr;
	
	FTimerHandle WeaponGrantTimerHandle;
	
private:
	void DebugDelayGrantWeapon1s();
public:
	UWeaponComponent();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UWeaponDefinition*> AvailableWeapons;
	
	
	UPROPERTY()
	FOnWeaponGranted OnWeaponGrantedDelegate;
	UPROPERTY()
	FOnWeaponInstanceUpdate OnWeaponInstanceUpdateDelegate;
	
	UFUNCTION()
	void OnFirePressed();
	UFUNCTION()
	void OnFireReleased();
	UFUNCTION()
	void GrantWeapon(UWeaponDefinition* Definition);
	UFUNCTION()
	void CycleFireMode();
	
};