// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractableComponent.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AMainPlayerController;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	UInteractableComponent* FocusedActorComponent;
public:	

	UPROPERTY()
	AMainPlayerController* PC =nullptr;
	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TArray<AActor*> NearbyInteractables;
	
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void HandleHighlight();
	UFUNCTION()
	UInteractableComponent* GetBestInteractable();
};
