// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InteractionDefinition.h"
#include "InteractionInstance.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoEntityUnderCursor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntityUnderCursor, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractableFocused,UInteractionDefinition*, Definition, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableFocuseEnd);

class AMainPlayerController;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TArray<TObjectPtr<UInteractionInstance>> InstancedInteractions;
	UPROPERTY()
	TObjectPtr<UInteractionDefinition> CurrentLoadedDefinition;	
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentFocusedInteractable;
	UPROPERTY()
	TWeakObjectPtr<AActor> PreviousFocusedInteractable;

	UPROPERTY()
	TWeakObjectPtr<AMainCharacter> Owner = nullptr;
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> EntityUnderCursor = nullptr;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>> NearbyInteractables;
public:	
	FOnEntityUnderCursor OnEntityUnderCursorDelegate;
	FOnNoEntityUnderCursor OnNoEntityUnderCursorDelegate;
	FOnInteractableFocused OnInteractableFocused;
	FOnInteractableFocuseEnd OnInteractableFocusEndDelegate;
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UInteractorComponent();
	UFUNCTION()
	void ExecuteInteraction(UInteractionDefinition* InteractionDefinition, AActor* Target);
	UFUNCTION()
	void QueryEntityUnderCursor();
	UFUNCTION()
	void Init(AMainCharacter* InOwner);
	UFUNCTION()
	void ProcessResultUnderCursor(bool bHit,  const FHitResult& Hit);
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void QueryInteractableUnderCursor();
	UFUNCTION()
	void HandleHighlight();
	UFUNCTION()
	void GetEntityData(AActor* Actor);
	UFUNCTION()
	AActor* GetBestInteractable();
	UFUNCTION()
	void AddInteractable(AActor* Actor);
	UFUNCTION()
	void RemoveInteractable(AActor* Actor);
};
