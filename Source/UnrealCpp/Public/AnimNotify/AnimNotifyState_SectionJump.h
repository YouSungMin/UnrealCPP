// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_SectionJump.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPP_API UAnimNotifyState_SectionJump : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) override;

	inline FName GetNextSectionName() const {return NextSectionName;}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SectionJump");
	FName NextSectionName;

private:
	UPROPERTY()
	TWeakObjectPtr<class AActionCharacter> OwnerCharacter = nullptr;
};
