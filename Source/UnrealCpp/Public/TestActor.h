// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class UNREALCPP_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Test_NormalFunction();

	UFUNCTION(BlueprintCallable, Category = "내 함수")
	void Test_UFunction();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = "내 함수")
	void Test_ImplementableFunction();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "내 함수")
	void Test_NativeEventFunction();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
