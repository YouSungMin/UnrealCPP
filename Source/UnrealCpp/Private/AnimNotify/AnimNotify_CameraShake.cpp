// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_CameraShake.h"

void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!CameraManager.IsValid())
	{
		UWorld* world = MeshComp->GetWorld();
		if (world)
		{
			CameraManager = world->GetFirstPlayerController()->PlayerCameraManager;
		}
	}

	if (CameraShake && CameraManager.IsValid())
	{
		CameraManager->StartCameraShake(CameraShake);
	}
}
