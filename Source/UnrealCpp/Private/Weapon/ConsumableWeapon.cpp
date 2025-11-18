// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ConsumableWeapon.h"

void AConsumableWeapon::OnAttack()
{
	RemainingUseCount--;
	if (RemainingUseCount <= 0)
	{
		OnWeaponUesEnded.Broadcast();
	}
}

void AConsumableWeapon::OnWeaponPickuped(AActionCharacter* InOwner)
{
	Super::OnWeaponPickuped(InOwner);

	RemainingUseCount = MaxUseCount;
}
