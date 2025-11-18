// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/UsedWeapon.h"

// Sets default values
AUsedWeapon::AUsedWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetMassOverrideInKg(NAME_None,70.0f);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	ForceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ForceLocation"));
	ForceLocation->SetupAttachment(Mesh);
}

void AUsedWeapon::BeginPlay()
{
	Super::BeginPlay();

	FVector dir = GetActorForwardVector() + GetActorUpVector();
	FVector wolrdLocation = GetActorLocation() + ForceLocation->GetRelativeLocation();

	Mesh->AddVelocityChangeImpulseAtLocation(dir*Force, wolrdLocation,NAME_None);
}


