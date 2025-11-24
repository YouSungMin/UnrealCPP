// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyPawn.h"
#include "Enemy/DamagePopupActor.h"
#include "Framework/DamagePopupSubsystem.h"
#include "Framework/EnemyCountSubsystem.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	PopupLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PopupLocation"));
	PopupLocation->SetupAttachment(Mesh);
	PopupLocation->SetRelativeLocation(FVector(0,0,100));

}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AEnemyPawn::OnTakeDamage);
	UEnemyCountSubsystem* countSystem = GetWorld()->GetSubsystem<UEnemyCountSubsystem>();

	if (UWorld* World = GetWorld())
	{
		EnemyCountSubsystem = World->GetSubsystem<UEnemyCountSubsystem>();
	}
}

// Called every frame
void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyPawn::ReturnToEnemyPool()
{
	// 1. 서브시스템이 유효한지 확인
	if (!EnemyCountSubsystem)
	{
		// 만약 캐싱되지 않았다면 다시 찾거나, 그냥 Destroy()를 호출하여 보험을 둡니다.
		if (UWorld* World = GetWorld())
		{
			EnemyCountSubsystem = World->GetSubsystem<UEnemyCountSubsystem>();
		}
	}

	// 2. 서브시스템을 통해 자신(this)을 풀로 반환
	if (EnemyCountSubsystem)
	{
		// 이 함수를 호출하면 Subsystem에서 활성 카운트를 줄이고 델리게이트를 발송합니다.
		EnemyCountSubsystem->ReturnToPool(this);

		// **중요:** 반환 후, 적을 비활성화된 초기 상태로 되돌리는 로직을 추가해야 합니다.
		// 예: 체력 초기화, 상태 초기화, 이펙트 종료 등
		// ResetState(); // <- 이런 초기화 함수를 호출해야 합니다.
	}
	else
	{
		// 최악의 경우 (서브시스템을 찾지 못함), 일반적인 파괴를 수행합니다.
		Destroy();
	}
}

void AEnemyPawn::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage();
	UE_LOG(LogTemp,Log,TEXT("Damage : %.1f"),Damage);

	//ADamagePopupActor* actor = GetWorld()->SpawnActor<ADamagePopupActor>(DamagePopupClass,
	//	PopupLocation->GetComponentToWorld());
	//if (actor)
	//{
	//	actor->PopupActivate(Damage);
	//}

	UDamagePopupSubsystem* popupSystem =  GetWorld()->GetSubsystem<UDamagePopupSubsystem>();
	popupSystem->ShowDamagePopup(Damage, PopupLocation->GetComponentLocation());

	UEnemyCountSubsystem* countSystem = GetWorld()->GetSubsystem<UEnemyCountSubsystem>();
	ReturnToEnemyPool();
}

