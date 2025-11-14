// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_AttackEnable.h"
#include "Player/ActionCharacter.h"

void UAnimNotifyState_AttackEnable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	}

	if (OwnerCharacter.IsValid())
	{	
		OwnerCharacter->OnAttackEnable(true);
	}
}

void UAnimNotifyState_AttackEnable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnAttackEnable(false);
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
