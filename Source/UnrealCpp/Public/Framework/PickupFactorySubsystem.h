// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PickupFactorySubsystem.generated.h"
/**
 * 
 */
 class APickup;
 enum class EItemCode : uint8;

UCLASS()
class UNREALCPP_API UPickupFactorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable)
	APickup* SpawnPickup(EItemCode ItemCode, const FVector& Location, const FRotator& Rotation );
};
