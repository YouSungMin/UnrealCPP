// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetupAttachment(root);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	WeaponCollision->SetupAttachment(WeaponMesh);
	WeaponCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeaponActor::OnWeaponBeginOverlap);
}

void AWeaponActor::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	if (WeaponOwner.IsValid())
	{
		if(WeaponOwner == OtherActor)
			return;
		//finalDamage += WeaponOwner->GetAttackPower();
		instigator = WeaponOwner->GetController();
	}
	UE_LOG(LogTemp,Log,TEXT("Overlapped : %s"),*OtherActor->GetName());
	UGameplayStatics::ApplyDamage(OtherActor,finalDamage,instigator,this,DamageType);
}

void AWeaponActor::AttackEbnale(bool bEnable)
{
	if (bEnable)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeaponActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// CDO(Class Default Object)의 설정대로 초기화된 이후 ( = OverlapOnlyPawn 설정 이후)
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

