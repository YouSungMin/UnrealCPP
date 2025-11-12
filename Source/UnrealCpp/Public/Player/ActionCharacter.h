// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ActionCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class UNREALCPP_API AActionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	// 이동 방향 입력 받기
	void OnMoveInput(const FInputActionValue& InValue);

	// 구르기 입력 받기
	void OnRollInput(const FInputActionValue& InValue);

	// 걷기 모드 설정
	void SetWalkMode();

	// 달리기 모드 설정
	void SetSprintMode();
private:
	void StaminaRegenTimerSet();

	void StaminaRegenPerTick();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;

	// Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Sprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Roll = nullptr;

	// AnimMotage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> RollMontage = nullptr;

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player|Movement")
	float WalkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player|Movement")
	float SprintSpeed = 1200.0f;

	// 현재 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Resource")
	float CurrentStamina = 100.0f;

	// 최대 스테미너
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float MaxStamina = 100.0f;

	// 달리기 상태일 때 초당 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float SprintStaminaCost = 20.0f;

	// 구르기를 하기 위해 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float RollStaminaCost = 50.0f;

	// 스테미너 리젠 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float StaminaRegenCoolTime = 3.0f;

	// 스테미너 초당 회복량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float StaminaRegenAmount = 50.0f;

	// 스테미너 틱당 회복량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float StaminaRegenAmountByTick = 10.0f;

	// 스테미너 틱당,최대치의 % 회복량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float StaminaRegenRatePerTick = 0.1f;

	// 플레이어가 뛰고있는 상태 표시 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprint = false;

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> AnimInstance = nullptr;
	
	float TimeSinceLastStaminaUse = 0.0f;
	FTimerHandle StaminaCoolTimer;
	FTimerHandle StaminaRegenTimer;
	bool bRegenStamina = false;
};
