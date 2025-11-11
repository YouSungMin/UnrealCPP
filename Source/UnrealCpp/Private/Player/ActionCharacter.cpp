// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->SocketOffset = FVector(0,0,250);
	SpringArm->bUsePawnControlRotation = true;	//스프링암의 회전을 컨트롤러에 맞춤

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(SpringArm);
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));

	bUseControllerRotationYaw = false;	// 컨트롤러의 Yaw회전을 사용안함
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 캐릭터 회전
	GetCharacterMovement()->RotationRate = FRotator(0,360,0);	


}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();	// ABP 가져오기
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsSprint)
	{
		Stamina -= RunStaminaCost;
		if (StaminaCheck())
		{
			IsSprint = false;
			SetWalkMode();
		}
	}
	else
	{
		Stamina += StaminaRegenRate;
		StaminaCheck();
	}
	UE_LOG(LogTemp, Log, TEXT("%.1f"),Stamina);
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhanced)	//입력 컴포넌트가 향상된 입력 컴포w넌트 일때
	{
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveInput);
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Started,
			[this](const FInputActionValue& _) {
				SetSprintMode();
			});
		enhanced->BindActionValueLambda(IA_Sprint, ETriggerEvent::Completed,
			[this](const FInputActionValue& _) {
				SetWalkMode();
			});
		enhanced->BindAction(IA_Roll, ETriggerEvent::Triggered, this, &AActionCharacter::OnRollInput);
	}
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	FVector2D inputDirection = InValue.Get<FVector2D>();
	FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);
	//UE_LOG(LogTemp, Log, TEXT("Dir : %s"), *inputDirection.ToString());
	FQuat controlYawRotation = FQuat(FRotator(0,GetControlRotation().Yaw,0));
	moveDirection = controlYawRotation.RotateVector(moveDirection);

	AddMovementInput(moveDirection);
}

void AActionCharacter::OnRollInput(const FInputActionValue& InValue)
{
	if (AnimInstance.IsValid())
	{
		if (!AnimInstance->IsAnyMontagePlaying() && Stamina >= RollStaminaCost)
		{
			if(!GetLastMovementInputVector().IsNearlyZero())
			{
				SetActorRotation(GetLastMovementInputVector().Rotation());
			}
			PlayAnimMontage(RollMontage);
			Stamina -= RollStaminaCost;
		}
	}
}

void AActionCharacter::SetWalkMode()
{
	IsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AActionCharacter::SetSprintMode()
{
	if (!GetLastMovementInputVector().IsNearlyZero())
	{
		IsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

bool AActionCharacter::StaminaCheck()
{
	if (Stamina <= 0)
	{
		IsStaminaZero = true;
		Stamina = 0;
	}
	else
	{
		IsStaminaZero = false;
		if (Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
		}
	}
	return IsStaminaZero;
}


