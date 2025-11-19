// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnum.h"
#include "Data/WeaponDataAssets.h"
#include "WeaponManagerComponent.generated.h"

class AWeaponActor;
class AUsedWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	AWeaponActor* GetEquippedWeapon(EItemCode InType) const;
	TSubclassOf<AUsedWeapon> GetUsedWeaponClass(EItemCode InType) const;
	TSubclassOf<APickup> GetPickupWeaponClass(EItemCode InType) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	// 시작 시 데이터 확인 및 로그 출력 함수
	void WeaponDataBaseCheck();
	
	// 시작 시 손에 장비하는 무기 액터들을 모두 생성하는 함수
	void SpawnWeaponInstances();
public:	
	
protected:
	// 컴포넌트가 관리하는 무기 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Database")
	TMap<EItemCode, TObjectPtr<UWeaponDataAssets>> WeaponDatabase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Instance")
	TMap<EItemCode, TObjectPtr<AWeaponActor>> WeaponInstances;
private:
	UPROPERTY()
	TWeakObjectPtr<class AActionCharacter> OwnerPlayer = nullptr;
};
