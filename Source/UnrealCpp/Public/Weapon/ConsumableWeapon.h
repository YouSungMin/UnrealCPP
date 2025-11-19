// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponActor.h"
#include "Item/Consumable.h"
#include "ConsumableWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUesEnded);
/**
 * 
 */
UCLASS()
class UNREALCPP_API AConsumableWeapon : public AWeaponActor
{
	GENERATED_BODY()
	
public:
	virtual void OnAttack() override;

	virtual void OnWeaponPickuped() override;

	virtual bool CanAttack()override { return (RemainingUseCount > 0);}

	//virtual FOnConsume& GetOnConsumeDelegate() { return OnWeaponUesEnded;}
protected:
	// 최대 사용 횟수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data");
	int32 MaxUseCount = 10;

	// 남은 사용 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data");
	int32 RemainingUseCount = 10;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Weapon");
	FOnConsume OnWeaponUesEnded;

};
