// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Pickup.h"
#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "NiagaraComponent.h"
#include "Player/InventoryOwner.h"

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
	//PickupOverlap->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	PickupOverlap->SetCollisionProfileName(TEXT("NoCollision"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(BaseRoot);

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupTimeline)
	{
		if (ScaleCurve)
		{
			FOnTimelineFloat UpdateDelegate;
			UpdateDelegate.BindUFunction(this, FName("OnTimeLineUpdate"));
			PickupTimeline->AddInterpFloat(DistanceCurve, UpdateDelegate);

			FOnTimelineEvent FinishDelegate;
			FinishDelegate.BindUFunction(this, FName("OnTimeLineFinished"));
			PickupTimeline->SetTimelineFinishedFunc(FinishDelegate);

		}

		PickupTimeline->SetPlayRate(1/Duration);
	}
	FTimerManager& timemanager = GetWorldTimerManager();
	timemanager.ClearTimer(PickupableTimer);
	timemanager.SetTimer(
		PickupableTimer,
		[this]() {
			PickupOverlap->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
		},
		PickupableTime,false);

	bPickuped = false;
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkeletalMesh->AddWorldRotation(FRotator(0, DeltaTime*RotateSpeed, 0));
}

void APickup::OnPickup_Implementation(AActor* Target)
{
	//UE_LOG(LogTemp,Log,TEXT("OnPickup_Implementation 실행"));
	if(!bPickuped)
	{
		bPickuped = true;
		PickupOwner = Target;
		PickupStartLocation = SkeletalMesh->GetRelativeLocation() + GetActorLocation();

		BaseRoot->SetSimulatePhysics(false);
		SetActorEnableCollision(false);
		//BaseRoot->SetCollisionProfileName(TEXT("NoCollision"));

		PickupTimeline->PlayFromStart(); // 타임라인 시작
	}
}

void APickup::AddImpulse(FVector& Velocity)
{
	BaseRoot->AddImpulse(Velocity, NAME_None, true);
}

void APickup::OnTimeLineUpdate(float Value)
{
	// 타임라인의 정규화 된 진행 시간 (0~1)
	float currentTime = PickupTimeline->GetPlaybackPosition();

	// 커브의 현재 값 받아오기
	float distanceValue = Value;
	float relativeValue = RelativeCurve ? RelativeCurve->GetFloatValue(currentTime) : 0.0f;
	float scaleValue = ScaleCurve ? ScaleCurve->GetFloatValue(currentTime) : 1.0f;

	// 커브값을 기준으로 새 위치와 스케일 계산
	FVector NewLocation = FMath::Lerp(PickupStartLocation, PickupOwner.Get()->GetActorLocation(), distanceValue);
	NewLocation += relativeValue * PickupHeight * FVector::UpVector;
	SkeletalMesh->SetWorldLocation(NewLocation);

	FVector NewScale = FVector::One() * scaleValue;
	SkeletalMesh->SetRelativeScale3D(NewScale);
}

void APickup::OnTimeLineFinished()
{
	if (PickupOwner.IsValid() && PickupOwner->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupOwner.Get(), PickupItem);
	}
	Destroy();
}
