// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyCountSubsystem.generated.h"


class AEnemyPawn;
// 1. 적의 수가 변경되었을 때 발송할 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveEnemyCountChanged, int32, NewActiveCount);

// 2. 적의 수가 0이 되었을 때 발송할 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDefeated);
/**
 * 
 */
UCLASS()
class UNREALCPP_API UEnemyCountSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Count")
	AEnemyPawn* RequestEnemy(const FVector& Location, const FRotator& Rotation);

	void ReturnToPool(AEnemyPawn* ReturnActor);

	// 5. 현재 활성화된 적의 수를 반환하는 Getter
	UFUNCTION(BlueprintPure, Category = "EnemyPool")
	int32 GetActiveEnemyCount() const { return ActiveEnemyCount; }

	// 6. 델리게이트 노출 (블루프린트 사용 가능)
	UPROPERTY(BlueprintAssignable, Category = "EnemyPool")
	FOnActiveEnemyCountChanged OnActiveEnemyCountChanged;

	UPROPERTY(BlueprintAssignable, Category = "EnemyPool")
	FOnAllEnemiesDefeated OnAllEnemiesDefeated;
protected:
	UPROPERTY()
	TSubclassOf<AEnemyPawn> EnemyPawn = nullptr;
private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AEnemyPawn>> EnemyPool;

	void BroadcastCountChange(int32 Delta);

	int32 ActiveEnemyCount = 0;
};
