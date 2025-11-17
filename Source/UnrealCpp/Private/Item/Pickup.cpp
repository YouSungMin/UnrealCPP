// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickup.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseRoot = CreateDefaultSubobject<USphereComponent>(TEXT("BaseRoot"));
	SetRootComponent(BaseRoot);
	BaseRoot->InitSphereRadius(1.0f);
	BaseRoot->SetSimulatePhysics(true);
	BaseRoot->BodyInstance.bLockXRotation = true;
	BaseRoot->BodyInstance.bLockYRotation = true;
	BaseRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));


	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(BaseRoot);
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMesh->AddRelativeRotation(FRotator(0,0,-10));

	PickupOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Overlap"));
	PickupOverlap->SetupAttachment(BaseRoot);
	PickupOverlap->InitSphereRadius(100.0f);
	PickupOverlap->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(BaseRoot);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupOverlap)
	{
		PickupOverlap->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPickupBeginOverlap);
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkeletalMesh->AddWorldRotation(FRotator(0, DeltaTime*RotateSpeed, 0));
}

void APickup::OnPickup_Implementation()
{
	UE_LOG(LogTemp,Log,TEXT("OnPickup_Implementation 실행"));
}

void APickup::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Log,TEXT("Pickup overlap"));
}
