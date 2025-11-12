// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceComponent.generated.h"


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

	// 스테미너 추가 감소용 함수
	UFUNCTION(BlueprintCallable)
	void AddStamina(float InValue);

	// 스테미너가 충분한지 확인하는 함수
	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool HasEnoughStamina(float InValue) { return CurrentStamina >= InValue; }

private:
	void StaminaAutoRegenCoolTimerSet();
	void StaminaRegenPerTick();

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

private:
	//float TimeSinceLastStaminaUse = 0.0f;
	//bool bRegenStamina = false;

	FTimerHandle StaminaAutoRegenCoolTimer;	// 스테미너 자동 회복용 타이머 핸들
	FTimerHandle StaminaRegenTickTimer;		// 스테미너 자동 회복 시 틱별 타이머 핸들 
};
