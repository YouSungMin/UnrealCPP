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
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	UFUNCTION(BlueprintCallable)
	void SpawnPickup(EItemCode ItemCode, const FVector& Location, const FRotator& Rotation );
protected:
	UPROPERTY()
	TObjectPtr<UDataTable> DataTable = nullptr;
};
