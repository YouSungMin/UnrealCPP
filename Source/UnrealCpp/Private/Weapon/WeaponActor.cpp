// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Player/ActionCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/StatusComponent.h"

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
		// 스테이터스 컴포넌트가 있으면 공격력 가져와서 추가하기
		if (WeaponOwner->GetStatusComponent() != nullptr)
		{
			finalDamage += WeaponOwner->GetStatusComponent()->GetAttackPower();
		}
		instigator = WeaponOwner->GetController();
	}
	UE_LOG(LogTemp,Log,TEXT("Overlapped : %s"),*OtherActor->GetName());
	UGameplayStatics::ApplyDamage(OtherActor,finalDamage,instigator,this,DamageType);
}

void AWeaponActor::WeaponActivate(bool bActivate)
{
	//SetActorHiddenInGame(!bActivate); // 무기는 비지빌리티만 수정

	if (bActivate)
	{
		// 무기 활성화
		AttachToComponent(
			WeaponOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("hand_rSocket"));
		SetActorHiddenInGame(false);
		OnWeaponActivate();
	}
	else
	{
		// 무기 비활성화
		SetActorHiddenInGame(true);
		AttachToComponent(
			WeaponOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("root"));
		SetActorRelativeLocation(FVector(0,0,-10000.0f));
		OnWeaponDeActivate();
	}
}

void AWeaponActor::AttackEnable(bool bEnable)
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


void AWeaponActor::OnWeaponPickuped(int InCount)
{

}

void AWeaponActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// CDO(Class Default Object)의 설정대로 초기화된 이후 ( = OverlapOnlyPawn 설정 이후)
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

