// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/EnemyCountSubsystem.h"
#include "Settings/EnemyCountSettings.h"


void UEnemyCountSubsystem::BroadcastCountChange(int32 Delta)
{
	// 활성화된 적의 수 업데이트
	ActiveEnemyCount += Delta;

	// 델리게이트 발송
	OnActiveEnemyCountChanged.Broadcast(ActiveEnemyCount);

	// 적의 수가 0이 되었는지 확인하고 델리게이트 발송
	if (ActiveEnemyCount == 0)
	{
		OnAllEnemiesDefeated.Broadcast();
	}
}

void UEnemyCountSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UEnemyCountSettings* settings = GetDefault<UEnemyCountSettings>();
	if (settings && !settings->EnemyPawnClass.IsNull())
	{
		EnemyPawn = settings->EnemyPawnClass.LoadSynchronous();
	}
	ActiveEnemyCount = 0;
}

AEnemyPawn* UEnemyCountSubsystem::RequestEnemy(const FVector& Location, const FRotator& Rotation)
{
	AEnemyPawn* selected = nullptr;
	UWorld* world = GetWorld();

    if (EnemyPool.Num() > 0)
    {
        // 🔴 Pool의 맨 끝 요소(가장 최근에 반환된 요소)를 Pop으로 꺼냅니다.
        // TObjectPtr은 Pop()으로 반환 가능합니다.
        selected = EnemyPool.Pop();

        if (IsValid(selected))
        {
            // 재활용
            selected->SetActorHiddenInGame(false);
            selected->SetActorEnableCollision(true); // 충돌 활성화
            selected->SetActorLocationAndRotation(Location, Rotation);

            // 활성 적의 수 증가 및 델리게이트 발송
            BroadcastCountChange(1);
        }
        else
        {
            // 만약 Pool에 Invalid 객체가 있었다면, 제거하고 다시 시도합니다.
            return RequestEnemy(Location, Rotation);
        }
    }
    // 2. Pool이 비어있으면 새로 생성
    else if (EnemyPawn && world)
    {
        FActorSpawnParameters SpawnParams;

        selected = world->SpawnActor<AEnemyPawn>(EnemyPawn, Location, Rotation, SpawnParams);

        if (IsValid(selected))
        {
            // 활성 적의 수 증가 및 델리게이트 발송
            BroadcastCountChange(1);

#if WITH_EDITOR
            selected->SetFolderPath(FName("EnemyPool/Active"));
#endif
        }
    }

    return selected;
}

void UEnemyCountSubsystem::ReturnToPool(AEnemyPawn* ReturnActor)
{
    if (IsValid(ReturnActor) && !EnemyPool.Contains(ReturnActor))
    {
        // 비활성화 조치
        ReturnActor->SetActorHiddenInGame(true);
        ReturnActor->SetActorEnableCollision(false); // 충돌 비활성화
        // 월드 밖으로 이동하여 혹시 모를 간섭 방지
        ReturnActor->SetActorLocation(FVector(0, 0, -10000.f));

        // Pool에 추가
        EnemyPool.Add(ReturnActor);

        // 활성 적의 수 감소 및 델리게이트 발송
        BroadcastCountChange(-1);

#if WITH_EDITOR
        ReturnActor->SetFolderPath(FName("EnemyPool/Pooled"));
#endif

        UE_LOG(LogTemp, Log, TEXT("적 반환 완료. 활성 적 수: %d, 풀 크기: %d"), ActiveEnemyCount, EnemyPool.Num());
    }
}
