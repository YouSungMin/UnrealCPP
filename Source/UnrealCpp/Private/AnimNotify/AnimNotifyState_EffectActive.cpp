// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotifyState_EffectActive.h"
#include "Player/ActionCharacter.h"

void UAnimNotifyState_EffectActive::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!OwnerCharacter.IsValid())
	{
		OwnerCharacter = Cast<AActionCharacter>(MeshComp->GetOwner());
	}

	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnWeaponEffectActivate(true);
	}
}

void UAnimNotifyState_EffectActive::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->OnWeaponEffectActivate(false);
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
