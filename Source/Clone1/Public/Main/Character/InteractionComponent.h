// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoEntityUnderCursor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntityUnderCursor, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFocused, AActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableFocuseEnd);

class AMainPlayerController;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentFocusedInteractable;
	UPROPERTY()
	TWeakObjectPtr<AActor> PreviousFocusedInteractable;
public:	
	FOnEntityUnderCursor OnEntityUnderCursorDelegate;
	FOnNoEntityUnderCursor OnNoEntityUnderCursorDelegate;
	FOnInteractableFocused OnInteractableFocused;
	FOnInteractableFocuseEnd OnInteractableFocusEndDelegate;
	
	UPROPERTY()
	TWeakObjectPtr<AMainCharacter> Owner = nullptr;
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> EntityUnderCursor = nullptr;
	UPROPERTY(VisibleAnywhere)
	TArray<TWeakObjectPtr<AActor>> NearbyInteractables;
	
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	UInteractionComponent();
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
};
