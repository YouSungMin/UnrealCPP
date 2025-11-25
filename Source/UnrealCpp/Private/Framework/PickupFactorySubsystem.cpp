// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PickupFactorySubsystem.h"
#include "Item/Pickup.h"

APickup* UPickupFactorySubsystem::SpawnPickup(EItemCode ItemCode, const FVector& Location, const FRotator& Rotation)
{
	//GetWorld()->SpawnActor<APickup>(
	//	ItemCode,
	//	Location,
	//	Rotation);
	return nullptr;
}
