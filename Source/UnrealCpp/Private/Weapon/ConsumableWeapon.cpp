// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ConsumableWeapon.h"

void AConsumableWeapon::OnAttack()
{
	RemainingUseCount--;
	UE_LOG(LogTemp,Log,TEXT("%d"), RemainingUseCount);
	if (RemainingUseCount <= 0)
	{
		OnWeaponUesEnded.Broadcast(WeaponID);
	}
}

void AConsumableWeapon::OnWeaponPickuped()
{
	Super::OnWeaponPickuped();

	RemainingUseCount = MaxUseCount;
}
