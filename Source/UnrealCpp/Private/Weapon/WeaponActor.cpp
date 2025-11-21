// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponActor.h"
#include "Components/CapsuleComponent.h"
#include "Player/ActionCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/StatusComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

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

	WeaponSlashEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	WeaponSlashEffect->SetupAttachment(WeaponMesh);
	WeaponSlashEffect->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeaponActor::OnWeaponBeginOverlap);
}

void AWeaponActor::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	DamageToTarget(OtherActor);
}

void AWeaponActor::DamageToTarget(AActor* InTarget)
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	if (WeaponOwner.IsValid())
	{
		if (WeaponOwner == InTarget)
			return;
		// 스테이터스 컴포넌트가 있으면 공격력 가져와서 추가하기
		if (WeaponOwner->GetStatusComponent() != nullptr)
		{
			finalDamage += WeaponOwner->GetStatusComponent()->GetAttackPower();
		}
		instigator = WeaponOwner->GetController();
	}
	//UE_LOG(LogTemp, Log, TEXT("Overlapped : %s"), *InTarget->GetName());
	UGameplayStatics::ApplyDamage(InTarget, finalDamage, instigator, this, DamageType);
}

void AWeaponActor::DamageToArea()
{
	float finalDamage = Damage;
	AController* instigator = nullptr;
	if (WeaponOwner.IsValid())
	{
		if (WeaponOwner->GetStatusComponent() != nullptr)
		{
			finalDamage += WeaponOwner->GetStatusComponent()->GetAttackPower();
		}
		instigator = WeaponOwner->GetController();
	}
	finalDamage *= 2.0f;


	FVector center = FMath::Lerp(
		WeaponMesh->GetSocketLocation(TEXT("BladeBase")),
		WeaponMesh->GetSocketLocation(TEXT("BladeTip")),
		0.5f);

	TArray<AActor*> IgnoreActors = { WeaponOwner.Get(), this};

	// 디버그 정보 그리기
	DrawDebugSphere(
		GetWorld(),
		center,				// 구의 중심점
		AreaInnerRadius,	// 구의 반지름
		12,					// 구를 쪼개는 수
		FColor::Red,		// 색상
		false,				// 안지워지게 할지
		DebugDuration,		// 몇초동안 보이게 할지
		0,					// 그리는 우선순위(0이 제일앞)
		1.0f				// 선 두께
	);
	DrawDebugSphere(
		GetWorld(),
		center,				// 구의 중심점
		AreaOuterRadius,	// 구의 반지름
		12,					// 구를 쪼개는 수
		FColor::Green,		// 색상
		false,				// 안지워지게 할지
		DebugDuration,		// 몇초동안 보이게 할지
		0,					// 그리는 우선순위(0이 제일앞)
		1.0f				// 선 두께
	);

	if (AreaAttackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AreaAttackEffect,
			center,
			WeaponOwner->GetActorRotation()
			);
	}

	// 범위로 데미지 주기
	UGameplayStatics::ApplyRadialDamageWithFalloff(
		GetWorld(),
		finalDamage,
		Damage,
		center,
		AreaInnerRadius,
		AreaOuterRadius,
		Falloff,
		DamageType,
		IgnoreActors,
		this,
		WeaponOwner->GetController(),
		ECollisionChannel::ECC_Pawn
		);
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

		AttackEnable(false);
		EffectActivate(false);

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

void AWeaponActor::EffectActivate(bool bActivate)
{
	if (bActivate)
	{
		WeaponSlashEffect->Activate(true);
	}
	else
	{
		WeaponSlashEffect->Deactivate();
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

