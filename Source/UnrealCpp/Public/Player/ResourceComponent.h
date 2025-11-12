// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"

//FOnStaminaEmpty 이름의 델리게이트가 있다라고 타입을 선언한것
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaEmpty);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie); // 사망 확인용
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, Current, float, Max); // 스테미너 변경 알림용
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float); // 체력 변경 알림용, 비교용


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPP_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 스테미너 추가 / 감소용 함수
	UFUNCTION(BlueprintCallable)
	void AddStamina(float InValue);

	// 스테미너가 충분한지 확인하는 함수
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool HasEnoughStamina(float InValue) const { return CurrentStamina >= InValue; }

	// 스테미너가 다 떨어졌음을 알리는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStaminaEmpty OnStaminaEmpty;

	// Health 추가 / 감소용 함수
	UFUNCTION(BlueprintCallable)
	void AddHealth(float InValue);

	// Healt가 충분한지 확인하는 함수
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool IsAlive() const { return CurrentHealth > 0; }

	inline float GetCurrentHealth() const { return CurrentHealth; };
	inline float GetMaxHealth() const { return MaxHealth;};
	inline float GetCurrentStamina() const { return CurrentStamina; };
	inline float GetMaxStamina() const { return MaxStamina;};

	// 사망을 알리는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnDie OnDie;

	// 스테미너 변화를 알리는 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStaminaChanged OnStaminaChanged;

	// 체력 변화를 알리는 델리게이트(일반 델리게이트는 블루프린트에서 사용 불가)
	FOnHealthChanged OnHealthChanged;

private:
	void StaminaAutoRegenCoolTimerSet();
	void StaminaRegenPerTick();

	inline void SetCurrentHealth(float InValue){
		CurrentHealth = InValue;
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	};
	inline void SetCurrentStamina(float InValue) {
		CurrentStamina = InValue;
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	};;
protected:
	// 현재 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stamina")
	float CurrentStamina = 100.0f;

	// 최대 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Stamina")
	float MaxStamina = 100.0f;

	// 스테미너 리젠 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Stamina")
	float StaminaRegenCoolTime = 3.0f;

	// 스테미너 초당 회복량
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Stamina")
	//float StaminaRegenAmount = 50.0f;

	// 스테미너 틱 간격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Stamina")
	float StaminaTickInterval = 0.1f;

	// 스테미너 틱당 회복량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Stamina")
	float StaminaRegenAmountByTick = 10.0f;

	// 스테미너 틱당,최대치의 % 회복량
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data|Stamina")
	//float StaminaRegenRatePerTick = 0.1f;

	// 현재 Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Health")
	float CurrentHealth = 100.0f;

	// 최대 Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data|Health")
	float MaxHealth = 100.0f;

private:
	//float TimeSinceLastStaminaUse = 0.0f;
	//bool bRegenStamina = false;

	FTimerHandle StaminaAutoRegenCoolTimer;	// 스테미너 자동 회복용 타이머 핸들
	FTimerHandle StaminaRegenTickTimer;		// 스테미너 자동 회복 시 틱별 타이머 핸들 
};
