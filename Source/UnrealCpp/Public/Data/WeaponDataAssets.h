// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Common/CommonEnum.h"
#include "WeaponDataAssets.generated.h"

class AWeaponActor;
class AUsedWeapon;
class APickup;
/**
 * 
 */
UCLASS()
class UNREALCPP_API UWeaponDataAssets : public UDataAsset
{
	GENERATED_BODY()
public:
	// 무기 종류(식별용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Type")
	EItemCode WeaponType = EItemCode::BasicWeapon;

	// 손에 들리는 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Classes")
	TSubclassOf<AWeaponActor> EquippedWeaponClass;

	// 사용 다한 무기 클래스(바닥에 버리는 용도, 장식용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Classes")
	TSubclassOf<AUsedWeapon> UsedWaeponClass;

	// 픽업 가능한 액터(바닥에 버리는 용도, 무기 획득용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actor Classes")
	TSubclassOf<APickup> PickupWeaponClass;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	bool IsValid() const
	{
		return EquippedWeaponClass != nullptr && UsedWaeponClass != nullptr && PickupWeaponClass != nullptr;
	}
};
