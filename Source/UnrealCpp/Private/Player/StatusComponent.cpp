// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/StatusComponent.h"

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

float UStatusComponent::GetAttackPower() const
{
	return Strength * AttackModifier;
}

float UStatusComponent::GetBaseMaxHealth() const
{
	return (Strength * 0.5 + Vitality * 0.5f) * 10.0f;
}

float UStatusComponent::GetBaseMaxStamina() const
{
	return (Strength * 0.2 + Vitality * 0.8f) * 10.0f;
}


