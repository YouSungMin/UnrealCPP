// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ConsumableWeapon.h"
#include "Components/SphereComponent.h"

void AConsumableWeapon::OnAttack()
{
	RemainingUseCount--;
	UE_LOG(LogTemp,Log,TEXT("%d"), RemainingUseCount);
	if (RemainingUseCount <= 0)
	{
		OnWeaponUesEnded.Broadcast(WeaponID);
	}
}

void AConsumableWeapon::OnWeaponPickuped(int InCount)
{
	Super::OnWeaponPickuped(InCount);

	RemainingUseCount = InCount;
}

void AConsumableWeapon::BeginPlay()
{
	Super::BeginPlay();
	RemainingUseCount = MaxUseCount;
}

void AConsumableWeapon::OnWeaponActivate()
{
	RemainingUseCount = MaxUseCount;
}
