// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();
	
	//BodyMesh->SetRelativeLocation(FVector(0,0,100));
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if(Updown)
	//{
	//	BodyMesh->AddRelativeLocation(DeltaTime * Speed *FVector::UpVector);
	//	if(BodyMesh->GetRelativeLocation().Z >= MoveHeight)
	//		Updown = false;
	//}
	//else
	//{
	//	BodyMesh->AddRelativeLocation(DeltaTime * Speed *FVector::DownVector);
	//	if (BodyMesh->GetRelativeLocation().Z <= 50.0f)
	//		Updown = true;
	//}

	//BodyMesh->AddRelativeRotation(DeltaTime * SpinSpeed *FRotator(0,1,0));

	ElapsedTime += DeltaTime;
	//UE_LOG(LogTemp,Log,TEXT("ElapsedTime : %.2f"),ElapsedTime)
	float cosValue = FMath::Cos(ElapsedTime); // 1 -> -1 -> 1
	cosValue += 1;			 // 2 -> 0 -> 2
	cosValue *= 0.5f;		 // 1 -> 0 -> 1
	cosValue = 1 - cosValue; // 0 -> 1 -> 0

	BodyMesh->SetRelativeLocation(FVector(0,0,cosValue*MoveHeight));
	BodyMesh->AddRelativeRotation(FRotator(0, SpinSpeed*DeltaTime, 0));
}

