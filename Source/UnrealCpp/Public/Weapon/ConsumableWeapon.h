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

	virtual void OnWeaponPickuped(int InCount) override;

	virtual bool CanAttack()override { return (RemainingUseCount > 0);}

	int32 GetRemainingUseCount() const { return RemainingUseCount; }

	//virtual FOnConsume& GetOnConsumeDelegate() { return OnWeaponUesEnded;}
protected:
	virtual void BeginPlay() override;
	virtual void OnWeaponActivate() override;
protected:
	// 최대 사용 횟수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data");
	int32 MaxUseCount = 10;

	// 남은 사용 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data");
	int32 RemainingUseCount = 10;

	// 무기를 다사용했을 때 사용되는 델리게이트 (사용안하는중)
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = "Weapon");
	FOnConsume OnWeaponUesEnded;

};
