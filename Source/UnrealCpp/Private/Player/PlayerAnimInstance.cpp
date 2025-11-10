// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	PlayerPawn = TryGetPawnOwner();
	if(PlayerPawn)
	{
		PlayerCharacter = Cast<ACharacter>(PlayerPawn);
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter)
	{
		UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
		if (MovementComponent)
		{
			FVector CurrentVelocity = MovementComponent->GetLastUpdateVelocity();
			CurrentVelocity.Z = 0.0f;
			float horizontalSpeed = CurrentVelocity.Size();

			UE_LOG(LogTemp, Log,TEXT("%f"),Speed);
			Speed = horizontalSpeed;
		}
	}
}
