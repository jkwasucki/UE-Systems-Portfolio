// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorInteractionComponent.h"
#include "PacMan/PacmanGame.h"
#include "PacMan/PacmanHUD.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "ArcadeMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuit);
class APacmanGame;
UCLASS()
class CLONE1_API AArcadeMachine : public AActor,public IInteractableInterface
{
	GENERATED_BODY()


protected:
	UPROPERTY()
	bool bIsActive = false;
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UActorInteractionComponent> InteractionComponent;
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> PacmanHUDClass;
	UPROPERTY(VisibleAnywhere, Category="UI")
	TObjectPtr<UWidgetComponent> PacmanHUDComponent;
	
public:	
	UPROPERTY()
	TWeakObjectPtr<UInputHandlerComponent> InputHandlerComponent = nullptr;
	FOnQuit OnQuitDelegate;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Arcade")
	TObjectPtr<APacmanGame> PacmanGame = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:
	AArcadeMachine();
	virtual TSoftObjectPtr<UInteractionDefinition> GetInteractionDefinition_Implementation() override;
	virtual void OnInteractionExecuted_Implementation() override;
	UFUNCTION()
	void SetupArcade(UInputHandlerComponent* inInputHandlerComponent);
	
	
	// Game switchers
	UFUNCTION()
	void Quit();
	UFUNCTION()
	void Start();
	
	UFUNCTION()
	APacmanGame* GetPacmanGame();
};
