// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyCountSubsystem.generated.h"


class AEnemyPawn;
// 1. 적의 수가 변경되었을 때 발송할 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChanged, int32, NewCount);

// 2. 적의 수가 0이 되었을 때 발송할 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemyClear);
/**
 * 
 */
UCLASS()
class UNREALCPP_API UEnemyCountSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;
	virtual void Deinitialize()override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	void RegistEnemy();

	void UnregistEnemy();

	UFUNCTION(BlueprintPure, Category = "Enemy Count")
	int32 GetCurrentEnemyCount() const { return CurrentEnemyCount; }

	// 적 수가 변경되었을 때 실행할 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Enemy Count")
	FOnEnemyCountChanged OnEnemyCountChanged;

	// 적이 모두 사라졌을 때 실행될 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Enemy Count")
	FOnAllEnemyClear OnAllEnemyClear;

private:

	int32 CurrentEnemyCount = 0;
};
