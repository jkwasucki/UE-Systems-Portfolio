// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/FItemStack.h"
#include "Interfaces/InteractableInterface.h"
#include "Components/ActorInteractionComponent.h"
#include "Components/SphereComponent.h"
#include "Item.generated.h"

UCLASS()
class CLONE1_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UActorInteractionComponent* InteractableComponent;
	UFUNCTION()
	void OnHighlight(bool bState);
	

public:
	UPROPERTY(EditAnywhere)
	UMaterial* M_Outline;

	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collider;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;
	
	FTimerHandle PhysicsSettleTimer;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FItemStack ItemStack;
	
	UFUNCTION()
	void EnablePhysics();
	UFUNCTION()
	void DisablePhysics();
	UFUNCTION(BlueprintCallable)
	void InitItem(const FItemStack& InStack);
	
	// Interactable Interface 
	virtual TSoftObjectPtr<UInteractionDefinition> GetInteractionDefinition_Implementation() override;
	virtual void Highlight_Implementation(bool bState) override;
	virtual bool IsInteractedWith_Implementation() override;
};
