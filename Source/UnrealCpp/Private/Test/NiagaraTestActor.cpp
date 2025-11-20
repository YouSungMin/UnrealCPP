// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/NiagaraTestActor.h"
#include "NiagaraComponent.h"

// Sets default values
ANiagaraTestActor::ANiagaraTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(Root);
	Effect->SetRelativeLocation(FVector(0,0,100));
	Effect->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ANiagaraTestActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANiagaraTestActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!Effect->IsActive())
	{
		ColorChange(DamageType);
		Effect->Activate();
	}
}

// Called every frame
void ANiagaraTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANiagaraTestActor::TestColorChange(FLinearColor InColor)
{
	Effect->SetColorParameter(TEXT("EffectColor"), InColor);
	//Effect->SetNiagaraVariableLinearColor(TEXT("EffectColor"), InColor);

}

void ANiagaraTestActor::ColorChange(EDamageType InType)
{
	if (!Effect)
	{
		return;
	}
	FLinearColor newColor;

	switch (InType)
	{
	case EDamageType::Normal:
		newColor = FLinearColor::White;
		break;
	case EDamageType::Fire:
		newColor = FLinearColor::Red;
		break;
	case EDamageType::Water:
		newColor = FLinearColor::Blue;
		break;
	default:
		break;
	}
	Effect->SetNiagaraVariableLinearColor(TEXT("EffectColor"), newColor);
}

