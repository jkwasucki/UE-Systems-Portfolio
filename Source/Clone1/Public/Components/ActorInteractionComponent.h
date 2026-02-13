// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "ActorInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractable_Interact, ACharacter*, Char);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractable_Highlight, bool, State);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UActorInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorInteractionComponent();

protected:

	UPROPERTY()
	TWeakObjectPtr<AActor> Parent = nullptr;
	UPROPERTY()
	USphereComponent* Collider = nullptr;
	
	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnBeginOverlap(   
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	
public:	
	
	UPROPERTY()
	bool bIsInteractedWith = false;
	UPROPERTY(EditAnywhere)
	FText ActionText;
	UPROPERTY(EditAnywhere)
	FText ActionKey;
	

	FOnInteractable_Interact OnInteractDelegate;
	FOnInteractable_Highlight OnHighlightDelegate;
	
	UFUNCTION()
	void SetupComponent(AActor* inParent,USphereComponent* inCollider);
};
