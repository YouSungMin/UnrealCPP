// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ResourceComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	CurrentStamina = MaxStamina; // 시작 할 때 최대치로 리셋
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceComponent::AddStamina(float InValue)
{
	// 스테미너 변경 처리
	CurrentStamina += InValue;

	//TimeSinceLastStaminaUse = 0;	// 시간을 직접 제이할 때 쓰던 코드(예시 확인용)

	// 스테미너를 소비하고 일정 시간 뒤에 자동 재생되게 타이머 세팅
	StaminaAutoRegenCoolTimerSet();

	if (CurrentStamina <= 0)
	{
		CurrentStamina = 0.0f;
		// 델리게이트로 스테미너가 떨어졌음을 알림
		OnStaminaEmpty.Broadcast();
	}
	UE_LOG(LogTemp, Log, TEXT("%.1f"), CurrentStamina);
}

void UResourceComponent::StaminaAutoRegenCoolTimerSet()
{
	UWorld* world = GetWorld();
	FTimerManager& timeManager = world->GetTimerManager();

		timeManager.SetTimer(
		StaminaAutoRegenCoolTimer,
		[this]() {
			//bRegenStamina = true;
			UWorld* world = GetWorld();
			FTimerManager& timeManager = world->GetTimerManager();
			timeManager.SetTimer(
				StaminaRegenTickTimer,
				this,
				&UResourceComponent::StaminaRegenPerTick,
				StaminaTickInterval,	// 실행 간격
				true,	// 반복 재생
				StaminaTickInterval);	// 첫 딜레이
		},
		StaminaRegenCoolTime,
		false);
}

void UResourceComponent::StaminaRegenPerTick()
{
	CurrentStamina += StaminaRegenAmountByTick;	// 틱당 10
	//CurrentStamina += MaxStamina * StaminaRegenRatePerTick; // 틱당 최대 스태미너의 10%

	if (CurrentStamina > MaxStamina)
	{
		CurrentStamina = MaxStamina;
		UWorld* world = GetWorld();
		FTimerManager& timeManager = world->GetTimerManager();
		timeManager.ClearTimer(StaminaRegenTickTimer);
	}
	UE_LOG(LogTemp, Log, TEXT("%.1f"), CurrentStamina);
}