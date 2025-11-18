// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsedWeapon.generated.h"

UCLASS()
class UNREALCPP_API AUsedWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsedWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> ForceLocation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Force = 300.0f;
};
