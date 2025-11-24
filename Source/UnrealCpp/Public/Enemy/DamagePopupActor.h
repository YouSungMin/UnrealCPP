// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagePopupActor.generated.h"

UCLASS()
class UNREALCPP_API ADamagePopupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagePopupActor();

	UFUNCTION(BlueprintCallable)
	void PopupActivate(float Damage = 1.0f);

	UFUNCTION(BlueprintCallable)
	void PopupDeactivate();

protected:
	virtual void BeginPlay() override;
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UWidgetComponent> DamageWidgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting", meta = (ClampMin = "1.0"))
	float LifeTime = 1.5f;

private:
	FTimerHandle LifeTimerHandle;

	TObjectPtr<class UDamageWidget> DamageWidget = nullptr;
};
