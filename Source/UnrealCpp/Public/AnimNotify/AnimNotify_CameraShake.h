// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UCameraShakeBase> CameraShake = nullptr;

private:
	TWeakObjectPtr<APlayerCameraManager> CameraManager = nullptr;

};
