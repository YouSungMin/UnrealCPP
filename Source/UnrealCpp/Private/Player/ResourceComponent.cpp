// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ResourceComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UResourceComponent::UResourceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	// 시작 할 때 최대치로 리셋
	SetCurrentHealth(MaxHealth);
	SetCurrentStamina(MaxStamina);
}


// Called every frame
void UResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceComponent::AddStamina(float InValue)
{
	//TimeSinceLastStaminaUse = 0;	// 시간을 직접 제이할 때 쓰던 코드(예시 확인용)
	// 스테미너 변경 처리
	SetCurrentStamina(FMath::Clamp(CurrentStamina + InValue, 0, MaxStamina));

	// 스테미너를 소비하고 일정 시간 뒤에 자동 재생되게 타이머 세팅
	StaminaAutoRegenCoolTimerSet();

	if (CurrentStamina <= 0)
	{
		// 델리게이트로 스테미너가 떨어졌음을 알림
		OnStaminaEmpty.Broadcast();
	}


	//UE_LOG(LogTemp, Log, TEXT("%.1f"), CurrentStamina);
}

void UResourceComponent::AddHealth(float InValue)
{
	float health = CurrentHealth + InValue;
	SetCurrentHealth(FMath::Clamp(health, 0, MaxHealth));

	if (!IsAlive())
	{
		OnDie.Broadcast();
	}
}

void UResourceComponent::StaminaAutoRegenCoolTimerSet()
{
	UWorld* world = GetWorld();
	FTimerManager& timeManager = world->GetTimerManager();

		timeManager.SetTimer(
		StaminaAutoRegenCoolTimer, 
		// StaminaAutoRegenCoolTimer 핸들에 연결될 타이머세팅, StaminaRegenCoolTime 초 후에 한번만 람다식을 실행
		[this]() {
			//bRegenStamina = true;
			UWorld* world = GetWorld();
			FTimerManager& timeManager = world->GetTimerManager();
			timeManager.SetTimer(
				StaminaRegenTickTimer,
				// StaminaRegenTickTimer 핸들에 연결될 타이머 세팅, 
				// StaminaTickInterval초를 처음에 한번 기다리고
				// StaminaTickInterval 시간 간격으로
				// StaminaRegenPerTick 함수 실행하는 타이머
				this,
				&UResourceComponent::StaminaRegenPerTick,
				StaminaTickInterval,	// 실행 간격
				true,					// 반복 재생
				StaminaTickInterval);	// 첫 딜레이
		},
		StaminaRegenCoolTime,
		false);
}

void UResourceComponent::StaminaRegenPerTick()
{
	float stamina = CurrentStamina + StaminaRegenAmountByTick;	// 틱당 10
	//CurrentStamina += MaxStamina * StaminaRegenRatePerTick; // 틱당 최대 스태미너의 10%

	if (stamina > MaxStamina)
	{
		stamina = MaxStamina;
		UWorld* world = GetWorld();
		FTimerManager& timeManager = world->GetTimerManager();
		timeManager.ClearTimer(StaminaRegenTickTimer);
	}

	SetCurrentStamina(stamina);
	//UE_LOG(LogTemp, Log, TEXT("%.1f"), CurrentStamina);
}