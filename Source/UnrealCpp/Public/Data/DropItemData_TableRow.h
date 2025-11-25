// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DropItemData_TableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDropItemData_TableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	TSubclassOf<class APickup> DropItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1"));
	float DropRate = 1.0f;
};
