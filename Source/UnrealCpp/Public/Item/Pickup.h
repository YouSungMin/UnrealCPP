// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickupable.h"
#include "Pickup.generated.h"

UCLASS()
class UNREALCPP_API APickup : public AActor, public IPickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnPickup_Implementation()override;
private:
	UFUNCTION()
	void OnPickupBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
protected:
	// 물리 적용용 루트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> BaseRoot = nullptr;

	// 아이템 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	// 아이템 획득 처리용 오버랩 컬리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> PickupOverlap = nullptr;

	// 아이템 이펙트 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RotateSpeed = 180.0f;

};
