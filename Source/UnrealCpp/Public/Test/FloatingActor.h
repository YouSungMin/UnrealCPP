// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class UNREALCPP_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 스태틱매시 움직임 업데이트하는 함수
	virtual void OnFloatingMeshUpdate(float DeltaTime);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	//이동 속도
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Speed = 100.0f;

	//최대로 올라갈 높이
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveHeight = 300.0f;

	//회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpinSpeed = 360.0f;
	bool Updown = true;
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BodyMesh = nullptr;

	//누적 시간
	float ElapsedTime = 0.0f;
};
