// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay(); 

	UE_LOG(LogTemp, Log, TEXT("Hello Unreal, 일반 로그"));
	UE_LOG(LogTemp, Warning, TEXT("Hello Unreal, 경고 로그"));
	UE_LOG(LogTemp, Error, TEXT("Hello Unreal, 에러 로그"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

