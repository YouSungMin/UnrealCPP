// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DamagePopupSubsystem.generated.h"

class ADamagePopupActor;
/**
 * 
 */
UCLASS()
class UNREALCPP_API UDamagePopupSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	UFUNCTION(BlueprintCallable, Category = "Damage Popup Pool")
	void ShowDamagePopup(float Damage,const FVector& Location);

	void ReturnToPool(ADamagePopupActor* ReturnActor);

protected:
	UPROPERTY()
	TSubclassOf<ADamagePopupActor> PopupClass = nullptr;

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<ADamagePopupActor>> Pool;
};
