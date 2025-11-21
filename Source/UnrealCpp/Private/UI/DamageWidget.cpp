// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DamageWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::TakeDamage(float InDamage)
{
	DamageAmount->SetText(FText::AsNumber(InDamage));
}
