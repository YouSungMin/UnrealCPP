// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DamageWidget.h"
#include "Components/TextBlock.h"

void UDamageWidget::SetDamageText(float Damage)
{
	if (DamageText)
	{
		DamageText->SetText(FText::AsNumber(static_cast<int32>(Damage)));
	}
}

void UDamageWidget::PlayPopupAnimation()
{
	if (PopupAnimation)
	{
		PlayAnimation(PopupAnimation);
	}
}
