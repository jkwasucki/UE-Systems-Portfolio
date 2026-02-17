// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/WeaponSystemComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	DebugDelayGrantWeapon1s();
}

void UWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon->TickWeapon(DeltaTime);
}


void UWeaponComponent::OnFirePressed()
{
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon->StartFiring();
}

void UWeaponComponent::OnFireReleased()
{
	if (!IsValid(CurrentWeapon)) return;
	
	CurrentWeapon->StopFiring();
}

void UWeaponComponent::GrantWeapon(UWeaponDefinition* Definition)
{
	if (!AvailableWeapons.Contains(Definition)) return;
	
	APawn* Owner = Cast<APawn>(GetOwner());
	if (!Owner) return;
	
	// Instance is owned by the component
	if (UWeaponInstance* NewWeaponInstance = NewObject<UWeaponInstance>(this))
	{
		CurrentWeapon = NewWeaponInstance;
		CurrentWeapon->Init(Definition,GetOwner<APawn>());
		
		OnWeaponGrantedDelegate.Broadcast(NewWeaponInstance);
	}
}

void UWeaponComponent::CycleFireMode()
{
	if (!IsValid(CurrentWeapon)) return;
	CurrentWeapon->SetNextFireMode();
}


void UWeaponComponent::DebugDelayGrantWeapon1s()
{
	if (AvailableWeapons.Num() == 0) return;
	FTimerDelegate TimerDelegate;
	
	TimerDelegate.BindLambda([this]()
	{
		UWeaponDefinition* FirstWeapon = AvailableWeapons[0];
		GrantWeapon(FirstWeapon);
	});
	
	
	GetWorld()->GetTimerManager().SetTimer(
	WeaponGrantTimerHandle,
	TimerDelegate,
	1.f,
	false
	);
}