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
	PickupOverlap->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(BaseRoot);

	PickupTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PickupTimeline"));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupOverlap)
	{
		PickupOverlap->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPickupBeginOverlap);
	}
	if (PickupTimeline)
	{
		if (ScaleCurve)
		{
			FOnTimelineFloat ScaleUpdateDelegate;
			ScaleUpdateDelegate.BindUFunction(this,FName("OnScaleUpdate"));
			PickupTimeline->AddInterpFloat(ScaleCurve,ScaleUpdateDelegate);

			FOnTimelineFloat DistanceUpdateDelegate;
			DistanceUpdateDelegate.BindUFunction(this, FName("OnDistanceUpdate"));
			PickupTimeline->AddInterpFloat(DistanceCurve, DistanceUpdateDelegate);

			FOnTimelineFloat RelativeUpdateDelegate;
			RelativeUpdateDelegate.BindUFunction(this, FName("OnRelativeUpdate"));
			PickupTimeline->AddInterpFloat(RelativeCurve, RelativeUpdateDelegate);

			FOnTimelineEvent ScaleFinishDelegate;
			ScaleFinishDelegate.BindUFunction(this,FName("OnScaleFinish"));
			PickupTimeline->SetTimelineFinishedFunc(ScaleFinishDelegate);

			FOnTimelineEvent PostUpdateDelegate;
			PostUpdateDelegate.BindUFunction(this, FName("OnTimelineTick"));
			PickupTimeline->SetTimelinePostUpdateFunc(PostUpdateDelegate);
		}

		PickupTimeline->SetPlayRate(1/Duration);
	}
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
		PickupLocation = GetActorLocation();
		PickupTimeline->PlayFromStart(); // 타임라인 시작
	}
}

void APickup::OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp,Log,TEXT("Pickup overlap"));
}

void APickup::OnScaleUpdate(float Value)
{
	FVector NewScale = FVector::One() * Value;
	SetActorScale3D(NewScale);
}

void APickup::OnDistanceUpdate(float Value)
{
	DisctanceVector = FMath::Lerp(PickupLocation, PickupOwner->GetActorLocation(),Value);
}

void APickup::OnRelativeUpdate(float Value)
{
	RelativeVector = Value * 50.0f * FVector(0,1,0);
}

void APickup::OnScaleFinish()
{
	if (PickupOwner.IsValid() && PickupOwner->Implements<UInventoryOwner>())
	{
		IInventoryOwner::Execute_AddItem(PickupOwner.Get(), PickupItem);
	}
}

void APickup::OnTimelineTick()
{
	SetActorLocation(DisctanceVector+RelativeVector);
}
