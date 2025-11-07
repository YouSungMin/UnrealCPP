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
protected:
	int32 Data1 = 10;

	UPROPERTY(VisibleAnywhere, Category= "Test변수")
	int32 Data2 = 20;

	UPROPERTY(VisibleDefaultsOnly, Category = "Test변수")
	int32 Data3 = 30;

	UPROPERTY(VisibleInstanceOnly, Category = "Test변수")
	int32 Data4 = 40;

	UPROPERTY(EditAnywhere, Category = "Test변수")
	int32 Data5 = 50;

	UPROPERTY(EditDefaultsOnly, Category = "Test변수")
	int32 Data6 = 60;

	UPROPERTY(EditInstanceOnly, Category = "Test변수")
	int32 Data7 = 70;

	UPROPERTY(BlueprintReadOnly, Category= "Test변수")
	int32 Data8 = 80;

	UPROPERTY(BlueprintReadWrite, Category= "Test변수")
	int32 Data9 = 90;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Test변수")
	int32 Data10 = 100;
};
