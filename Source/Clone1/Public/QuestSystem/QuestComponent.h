// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Definitions/QuestDefinition.h"
#include "Instances/QuestInstance.h"
#include "QuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestStarted, UQuestInstance*, NewQuest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestEnded, UQuestInstance*, Quest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLONE1_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuestComponent();

protected:
	virtual void BeginPlay() override;
	FTimerHandle QuestStartTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UQuestInstance*> ActiveQuests;
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UQuestDefinition* DEBUG_SomeQuest;
	
	UPROPERTY()
	FOnQuestStarted OnQuestStartedDelegate;
	UPROPERTY()
	FOnQuestEnded OnQuestEndedDelegate;	
	
	UFUNCTION()
	void AddQuest(UQuestDefinition* InQuestDefinition);
	UFUNCTION()
	void RemoveQuest(UQuestInstance* InQuestInstance);
	UFUNCTION()
	void StartDebugQuest();
};
