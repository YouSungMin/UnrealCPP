// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Enemy/EnemyPawn.h"
#include "EnemyCountSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Enemy Count Settings"))
class UNREALCPP_API UEnemyCountSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "UI")
	TSoftClassPtr<AEnemyPawn> EnemyPawnClass = nullptr;
};
