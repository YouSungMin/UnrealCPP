// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ActionCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Weapon/WeaponActor.h"
#include "Player/ResourceComponent.h"
#include "Player/StatusComponent.h"
#include "Player/WeaponManagerComponent.h"
#include "Player/ActionPlayerController.h"
#include "Weapon/ConsumableWeapon.h"
#include "Item/Pickupable.h"
#include "Item/Pickup.h"
#include "Weapon/UsedWeapon.h"


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

	DropLocation = CreateDefaultSubobject<USceneComponent>(TEXT("DropLocation"));
	DropLocation->SetupAttachment(RootComponent);
	DropLocation->SetRelativeLocation(FVector(80.0f,30.0f,0.0f));

	Resource = CreateDefaultSubobject<UResourceComponent>(TEXT("PlayerResource"));
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("PlayerStatus"));
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));

	bUseControllerRotationYaw = false;	// 컨트롤러의 Yaw회전을 사용안함
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 캐릭터 회전
	GetCharacterMovement()->RotationRate = FRotator(0,360,0);	

}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	if (Resource)
	{
		Resource->OnStaminaEmpty.AddDynamic(this, &AActionCharacter::SetWalkMode);
		if (Status)
		{
			Resource->SetMaxHealth(Status->GetBaseMaxHealth());
			Resource->SetMaxStamina(Status->GetBaseMaxStamina());
		}
	}
	Super::BeginPlay();

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance(); // ABP 가져오기
	}

	bIsSprint = false;

	// 캐릭터가 다른 액터에 오버랩되었을 때 실행하기 위한 바인딩
	OnActorBeginOverlap.AddDynamic(this, &AActionCharacter::OnBeginOverlap);
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StandSprintStamina(DeltaTime);
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhanced = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhanced)	//입력 컴포넌트가 향상된 입력 컴포넌트 일때
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
		enhanced->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &AActionCharacter::OnAttackInput);
	}
}

void AActionCharacter::AddItem_Implementation(EItemCode Code, int32 Count)
{
	const UEnum* EnumPtr = StaticEnum<EItemCode>();
	
	UE_LOG(LogTemp,Log,TEXT("아이템 추가 : %s"), *EnumPtr->GetDisplayNameTextByValue(static_cast<int8>(Code)).ToString());
	EquipWeapon(Code);
	CurrentWeapon->OnWeaponPickuped(Count);
}

void AActionCharacter::EquipWeapon(EItemCode WeaponCode)
{
	if (CurrentWeapon.IsValid())
	{
		if (CurrentWeapon->GetWeaponID() != EItemCode::BasicWeapon
			&& CurrentWeapon->GetWeaponID() != WeaponCode
			&& CurrentWeapon->CanAttack())
		{
			DropCurrentWeapon(CurrentWeapon->GetWeaponID());
		}
		CurrentWeapon->WeaponActivate(false);
	}

	CurrentWeapon = WeaponManager->GetEquippedWeapon(WeaponCode);
	CurrentWeapon->WeaponActivate(true);

}

void AActionCharacter::DropWeapon(EItemCode WeaponCode)
{
	UE_LOG(LogTemp, Log, TEXT("다쓴 무기 버리기"));
	if (TSubclassOf<AUsedWeapon> usedClass = WeaponManager->GetUsedWeaponClass(WeaponCode))
	{
		GetWorld()->SpawnActor<AUsedWeapon>(
			*usedClass,
			DropLocation->GetComponentLocation(),
			GetActorRotation());
	}
}

void AActionCharacter::OnAttackEnable(bool bAttackEnable)
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->AttackEnable(bAttackEnable);
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
		if (!AnimInstance->IsAnyMontagePlaying() && Resource->HasEnoughStamina(RollStaminaCost))
		{
			if(!GetLastMovementInputVector().IsNearlyZero())
			{
				SetActorRotation(GetLastMovementInputVector().Rotation());
			}
			//TimeSinceLastStaminaUse = 0;
			Resource->AddStamina(-RollStaminaCost);
			PlayAnimMontage(RollMontage);
		}
	}
}

void AActionCharacter::OnAttackInput(const FInputActionValue& InValue)
{
	// 애님 인스턴스가 있고 스테미너가 충분하고 현재 무기가 공격을 할 수 있는 상태여야 함
	if (AnimInstance.IsValid() && Resource->HasEnoughStamina(AttackStaminaCost)
		&& CurrentWeapon.IsValid() && CurrentWeapon->CanAttack()) 
	{
		if (!AnimInstance->IsAnyMontagePlaying())	// 몽타주가 재생 중이 아닐 때
		{
			// 첫 번째 공격
			PlayAnimMontage(AttackMontage); // 몽타주 재생

			// 몽타주가 끝났을때 델리게이트 발송 (몽타주 플레이 이후 등록)
			FOnMontageEnded onMontageEnded;
			onMontageEnded.BindUObject(this, &AActionCharacter::OnAttachMontageEnded);
			AnimInstance->Montage_SetEndDelegate(onMontageEnded);

			Resource->AddStamina(-AttackStaminaCost);	//스태미너 감소
			if (CurrentWeapon.IsValid() && (CurrentWeapon->GetWeaponID() != EItemCode::BasicWeapon))
			{
				CurrentWeapon->OnAttack();
			}
		}
		else if (AnimInstance->GetCurrentActiveMontage() == AttackMontage)
		{
			SectionJumpForCombo();
		}
	}
	
}

void AActionCharacter::SetWalkMode()
{
	bIsSprint = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AActionCharacter::SetSprintMode()
{
	bIsSprint = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AActionCharacter::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Log, TEXT("Char overlap : other is %s"),*OtherActor->GetName());

	// Cast를 이용한 인터페이스 사용
	//IPickupable* test = Cast<IPickupable>(OtherActor);
	//if (test)
	//{
	//	IPickupable::Execute_OnPickup(OtherActor);	// 블루프린트 구현이 있을 경우, 블루프린트의 구현이 실행
	//	//test->OnPickup_Implementation();			// 블루프린트 구현 무시
	//}

	// Implements를 이용한 인터페이스 사용
	if (OtherActor->Implements<UPickupable>())	//otherActor가 IPickupable 인터페이스를 구현했는지 확인
	{
		IPickupable::Execute_OnPickup(OtherActor,this);	// 구현이 되어 있으면 실행
	}
}


void AActionCharacter::OnAttachMontageEnded(UAnimMontage* Motage, bool bInterrupted)
{
	UE_LOG(LogTemp,Log,TEXT("공격 몽타주가 끝남"));
	if (CurrentWeapon.IsValid() && !CurrentWeapon->CanAttack())
	{
		DropWeapon(CurrentWeapon->GetWeaponID());
		EquipWeapon(EItemCode::BasicWeapon);
	}
}

void AActionCharacter::SectionJumpForCombo()
{
	if (SectionJumpNotify.IsValid() && bComboReady)	//SectionJumpNotify가 있고 콤보가 가능한 상태이면
	{
		UAnimMontage* current = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_SetNextSection(					// 다음 섹션으로 점프
			AnimInstance->Montage_GetCurrentSection(current),	// 현재 섹션
			SectionJumpNotify->GetNextSectionName(),			// 실행될 몽타주
			current);
		bComboReady = false;	//중복 실행 방지
		Resource->AddStamina(-AttackStaminaCost);
		if (CurrentWeapon.IsValid() && (CurrentWeapon->GetWeaponID() != EItemCode::BasicWeapon))
		{
			CurrentWeapon->OnAttack();
		}
	}
}

void AActionCharacter::StandSprintStamina(float DeltaTime)
{
	if ((bIsSprint && !GetVelocity().IsNearlyZero())
		&& (AnimInstance.IsValid() && !AnimInstance->IsAnyMontagePlaying()))
	{ 
		Resource->AddStamina(-SprintStaminaCost * DeltaTime);
	}
}

void AActionCharacter::DropCurrentWeapon(EItemCode WeaponCode)
{
	if (CurrentWeapon.IsValid() && (CurrentWeapon->GetWeaponID() != EItemCode::BasicWeapon))
	{
		if (TSubclassOf<APickup> pickupClass = WeaponManager->GetPickupWeaponClass(WeaponCode))
		{
			APickup* pickup = GetWorld()->SpawnActor<APickup>(
				*pickupClass,
				DropLocation->GetComponentLocation(),
				GetActorRotation()
				);

			AConsumableWeapon* conWeapon = Cast<AConsumableWeapon>(CurrentWeapon);

			pickup->SetPickupCount(conWeapon->GetRemainingUseCount());

			FVector velocity = (GetActorForwardVector() + GetActorUpVector()) * 300.0f;
			pickup->AddImpulse(velocity);
		}
	}
}

void AActionCharacter::OnWeaponEffectActivate(bool bActivate)
{
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->EffectActivate(bActivate);
	}
}

void AActionCharacter::OnAreaAttack()
{
	//UE_LOG(LogTemp,Log,TEXT("AreaAttack"));
	if (CurrentWeapon.IsValid())
	{
		CurrentWeapon->DamageToArea();	
	}
}

void AActionCharacter::OnAreaAttakCameraShake()
{
	UE_LOG(LogTemp, Log, TEXT("Notify"));

	if (CurrentWeapon.IsValid())
	{
		//TSubclassOf<UCameraShakeBase> ShakeClass = CurrentWeapon->GetAreaAttakCameraShake();
		if (CurrentWeapon->GetAreaAttakCameraShake())
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				PC->ClientStartCameraShake(CurrentWeapon->GetAreaAttakCameraShake());
				UE_LOG(LogTemp, Log, TEXT("Shake"));
			}
		}
	}
}

void AActionCharacter::TestDropUsedWeapon()
{
	if (CurrentWeapon.IsValid())
	{
		DropWeapon(CurrentWeapon->GetWeaponID());
	}
}

void AActionCharacter::TestDropCurrentWeapon()
{
	DropCurrentWeapon(CurrentWeapon->GetWeaponID());
}




