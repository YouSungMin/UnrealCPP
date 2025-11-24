// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Enemy/DamagePopupActor.h"
#include "DamagePopupSettings.generated.h"

/**
 * 
 */

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Damage Popup Settings"))
class UNREALCPP_API UDamagePopupSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "UI")
	TSoftClassPtr<ADamagePopupActor> DamagePopupClass = nullptr;
};
