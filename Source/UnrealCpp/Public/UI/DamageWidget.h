// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void TakeDamage(float InDamage);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource", meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageAmount;
};
