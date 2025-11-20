#pragma once

#include "CoreMinimal.h"
#include "CommonEnum.generated.h"
/**
 * 아이템 코드 열거형
 */
UENUM(BlueprintType)
enum class EItemCode : uint8
{
    BasicWeapon    UMETA(DisplayName = "기본무기(도끼)"),
    Sword          UMETA(DisplayName = "대검"),  // Sowrd -> Sword 수정
    Hammer         UMETA(DisplayName = "망치"),  // Hamer -> Hammer 수정
};

/**
 * 비트 플래그 열거형
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EBitflag : uint8
{
    None = 0,
    Test1 = 1 << 0, // 1
    Test2 = 1 << 1, // 2
    Test3 = 1 << 2, // 4
    Test4 = 1 << 3, // 8
    Test5 = 1 << 4  // 16
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
    Normal UMETA(DisplayName = "무속성"),
    Fire UMETA(DisplayName = "불속성"),
    Water UMETA(DisplayName = "물속성")
};