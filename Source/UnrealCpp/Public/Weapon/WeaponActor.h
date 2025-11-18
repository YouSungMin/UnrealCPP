// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Player/ActionCharacter.h"
#include "WeaponActor.generated.h"

UCLASS()
class UNREALCPP_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:	
	// 공격을 활성화/비활성화 하는 함수  콜리전 키고 끄기
	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);

	// 공격을 했을 때 실행되어야 할 함수
	UFUNCTION(BlueprintCallable)
	virtual void OnAttack(){};
	
	// 무기를 획득했을 때 싫
	UFUNCTION(BlueprintCallable)
	virtual void OnWeaponPickuped(AActionCharacter* InOwner);

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componenets")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componenets")
	TObjectPtr<class UCapsuleComponent> WeaponCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UDamageType> DamageType = nullptr;
	
private:
	TWeakObjectPtr<AActionCharacter> WeaponOwner = nullptr;
};
