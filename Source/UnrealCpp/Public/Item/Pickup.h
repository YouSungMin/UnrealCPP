// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickupable.h"
#include "Common/CommonEnum.h"
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

	virtual void OnPickup_Implementation(AActor* Target)override;
private:
	UFUNCTION()
	void OnPickupBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnScaleUpdate(float Value);

	UFUNCTION()
	void OnDistanceUpdate(float Value);

	UFUNCTION()
	void OnRelativeUpdate(float Value);

	UFUNCTION()
	void OnScaleFinish();

	UFUNCTION()
	void OnTimelineTick();
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
	TObjectPtr<class UNiagaraComponent> Effect = nullptr;

	// 타임라인 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UTimelineComponent> PickupTimeline = nullptr;

	// 이 픽업을 먹었을 때 얻을 아이템 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	EItemCode PickupItem =  EItemCode::BasicWeapon;

	// 픽업 획득 효과용 스케일 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> ScaleCurve = nullptr;

	// 픽업 획득 효과용 스케일 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> RelativeCurve = nullptr;

	// 픽업 획득 효과용 스케일 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UCurveFloat> DistanceCurve = nullptr;

	// 아이템 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RotateSpeed = 180.0f;

	// 아이템 회전에 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float Duration = 0.5f;

protected:
	FVector RelativeVector;
	FVector DisctanceVector;
private:
	// 이 픽업 아이템을 먹은 액터
	TWeakObjectPtr<AActor> PickupOwner = nullptr;

	// 획득 되었는지 여부(true면 획득 처리 중)
	bool bPickuped = false;

	FVector PickupLocation;

};
