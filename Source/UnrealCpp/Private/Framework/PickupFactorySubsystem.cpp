// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PickupFactorySubsystem.h"
#include "Settings/PickupFactorySettings.h"
#include "Item/Pickup.h"

void UPickupFactorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UPickupFactorySettings* settings = GetDefault<UPickupFactorySettings>();
	if (settings && !settings->PickupDataTable.IsNull())
	{
		DataTable = settings->PickupDataTable.LoadSynchronous();
	}
}

void UPickupFactorySubsystem::SpawnPickup(EItemCode ItemCode, const FVector& Location, const FRotator& Rotation)
{
	if (DataTable == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("PickupTable is not loaded"));
		return;
	}

	const UEnum* ItemEnumPtr = StaticEnum<EItemCode>();
	if(ItemEnumPtr == nullptr) return;

	FName RowName = FName(*ItemEnumPtr->GetNameStringByValue((int64)ItemCode));

	static const FString ContextString(TEXT("PickupFactorySubsystem::SpawnPickup"));
	FDropItemData_TableRow* ItemData = DataTable->FindRow<FDropItemData_TableRow>(RowName, ContextString);

	if (ItemData && ItemData->DropItemClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// FActorSpawnParameters는 충돌 처리 등을 설정할 때 쓰는데, 기본값으로 둡니다.
			World->SpawnActor<APickup>(ItemData->DropItemClass, Location, Rotation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot find Item Data for Row: %s"), *RowName.ToString());
	}

}
