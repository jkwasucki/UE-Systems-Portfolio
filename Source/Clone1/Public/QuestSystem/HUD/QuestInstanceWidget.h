// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveInstanceWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "QuestSystem/Instances/QuestInstance.h"
#include "QuestInstanceWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLONE1_API UQuestInstanceWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION()
	void SetData();
	
	UPROPERTY()
	TArray<UObjectiveInstanceWidget*> ObjectiveInstanceWidgets;
public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UObjectiveInstanceWidget> ObjectiveInstanceWidgetClass;
	UPROPERTY()
	TWeakObjectPtr<UQuestInstance> ObservedQuestInstance;
	
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* QuestName;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ID;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* State;
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* ObjectivesList;
	
	UFUNCTION()
	void Init(UQuestInstance* QuestInstance);
	UFUNCTION()
	void DisplayObjectives();
};
