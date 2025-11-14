// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AnimNotify/AnimNotifyState_SectionJump.h"
#include "ActionCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UResourceComponent;
//class UAnimNotifyState_SectionJump;

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

	UResourceComponent* GetResourceComponent() { return Resource; }

	inline void SetSectionJumpNotify(UAnimNotifyState_SectionJump* InSectionJumpNotify)
	{
		SectionJumpNotify = InSectionJumpNotify;
		bComboReady = InSectionJumpNotify != nullptr;
	}

	// 노피파이가 공격을 가능하게 만들라는 신호가 왔을 때 실행될 함수
	void OnAttackEnable(bool bAttackEnable);

protected:
	// 이동 방향 입력 받기
	void OnMoveInput(const FInputActionValue& InValue);

	// 구르기 입력 받기
	void OnRollInput(const FInputActionValue& InValue);

	// 공격 입력 받기
	void OnAttackInput(const FInputActionValue& InValue);

	// 걷기 모드 설정 (다이나믹 델리게이트에 바인드하기 위해 UFUNTION 추가)
	UFUNCTION()
	void SetWalkMode();

	// 달리기 모드 설정
	void SetSprintMode();
private:
	// 콤보용 섹션 점프 함수
	void SectionJumpForCombo();

	// 달리기 스테미너 소비 함수
	void StandSprintStamina(float DeltaTime);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	TObjectPtr<class UResourceComponent> Resource = nullptr;

	// Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Sprint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Roll = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Attack = nullptr;

	// AnimMotage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> RollMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	// 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player|Movement")
	float WalkSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player|Movement")
	float SprintSpeed = 1200.0f;

	

	// 달리기 상태일 때 초당 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float SprintStaminaCost = 20.0f;

	// 구르기를 하기 위해 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float RollStaminaCost = 50.0f;

	// 공격하기 위한 스테미너 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Resource")
	float AttackStaminaCost = 15.0f;

	// 플레이어가 뛰고있는 상태 표시 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|State")
	bool bIsSprint = false;

	// 플레이어가 현재 가지고 있는 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
	TWeakObjectPtr<class AWeaponActor> CurrentWeapon = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> AnimInstance = nullptr;

	// 현재 진행중인 섹션점프 노티파이 스테이트
	UPROPERTY()
	TWeakObjectPtr<UAnimNotifyState_SectionJump> SectionJumpNotify = nullptr;

	// 콤보가 가능한 상황인지 확인하기 위한 플래그
	bool bComboReady = false;
};
