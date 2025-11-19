// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WeaponManagerComponent.h"
#include "Weapon/WeaponActor.h"
#include "Weapon/UsedWeapon.h"
#include "Item/Pickup.h"


// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	if (WeaponDatabase.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("무기 데이터베이스가 비었음!!"));
	}
	else
	{
		for (const auto& pair : WeaponDatabase)
		{
			if (!pair.Value)
			{
				UE_LOG(LogTemp,Error,TEXT("무기(%d)의 데이터가 null"),static_cast<int32>(pair.Key));
			}
			else if (!pair.Value->IsValid())
			{
				UE_LOG(LogTemp,Error,TEXT("무기(%d)의 데이터가 유효하지 않습니다"),static_cast<int32>(pair.Key));
			}
			else if (pair.Key != pair.Value->WeaponType)
			{
				UE_LOG(LogTemp,Warning,TEXT("무기(%d)의 키값과 데이터 타입이 서로 다릅니다"),static_cast<int32>(pair.Key));
			}
		}
	}
}


