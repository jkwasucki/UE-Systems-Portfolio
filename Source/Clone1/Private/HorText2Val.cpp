// Fill out your copyright notice in the Description page of Project Settings.


#include "HorText2Val.h"

void UHorText2Val::SetData(FText Name, FText Val)
{
	NameText->SetText(Name);
	ValueText->SetText(Val);
}

void UHorText2Val::UpdateValue(FText Value)
{
	ValueText->SetText(Value);
}
