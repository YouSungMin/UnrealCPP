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

	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 바라보게 회전
	GetCharacterMovement()->RotationRate = FRotator(0,360,0);


}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhanced)	//입력 컴포넌트가 향상된 입력 컴포w넌트 일때
	{
		enhanced->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AActionCharacter::OnMoveInput);
	}
}

void AActionCharacter::OnMoveInput(const FInputActionValue& InValue)
{
	MyController = GetController();
	if (MyController)
	{
		FRotator ControlRotation = MyController->GetControlRotation();
		FRotator YawRotation(0,ControlRotation.Yaw,0);
		FVector2D inputDirection = InValue.Get<FVector2D>();

		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		FVector ForwardDirection = YawRotation.Vector();
		//UE_LOG(LogTemp, Log, TEXT("Dir : %s"), *inputDirection.ToString());
		//FVector moveDirection(inputDirection.Y, inputDirection.X, 0.0f);
		AddMovementInput(ForwardDirection, inputDirection.Y);
		AddMovementInput(RightDirection, inputDirection.X);
	}
}

