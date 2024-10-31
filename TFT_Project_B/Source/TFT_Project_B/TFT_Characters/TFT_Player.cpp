// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Player.h"

#include "TFT_Creature.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "TFT_GameInstance.h"
#include "TFT_UIManager.h"
#include "TFT_SoundManager.h"
#include "TFT_EffectManager.h"

#include "TFT_InvenWidget.h"
#include "TFT_EquipmentWidget.h"
#include "TFT_Item.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ATFT_Player::ATFT_Player()
{
	_meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));

	_invenCom = CreateDefaultSubobject<UTFT_InvenComponent>(TEXT("Inven_Com"));

	SetMesh("/Script/Engine.SkeletalMesh'/Game/ControlRig/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'");

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArm->SetupAttachment(GetCapsuleComponent());
	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_camera->SetupAttachment(_springArm);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	_statCom->SetExp(0);
}

void ATFT_Player::BeginPlay()
{
	Super::BeginPlay();

	if (_invenCom != nullptr)
	{
		_invenCom->_itemAddedEvent.AddUObject(this, &ATFT_Player::AddItemHendle);
		_invenCom->_GoldChangeEvnet.AddUObject(this, &ATFT_Player::UIGold);
		/*UIMANAGER->_EquipmentCloseResetEvent.AddUObject(this, &ATFT_Player::CloseResetEquipment);
		UIMANAGER->GetInvenUI()->_SlotItemEvent.AddUObject(this, &ATFT_Player::DropItemPlayer);
		UIMANAGER->GetInvenUI()->_itemSellEvent.AddUObject(this, &ATFT_Player::SellItemPlayer);
		UIMANAGER->GetInvenUI()->_itemUesEvent.AddUObject(this, &ATFT_Player::UseItemPlayer);
		UIMANAGER->GetEquipmentUI()->_ItemChangeEvent.AddUObject(this, &ATFT_Player::ChangeEquipment);
		UIMANAGER->GetEquipmentUI()->_ItemChangeEvent_stat.AddUObject(this, &ATFT_Player::UseItemPlayer_Equipment);*/
	}
}

void ATFT_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATFT_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATFT_Player::Init()
{
}

void ATFT_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &ATFT_Player::Move);

		EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &ATFT_Player::Look);

		EnhancedInputComponent->BindAction(_jumpAction, ETriggerEvent::Started, this, &ATFT_Player::JumpA);

		EnhancedInputComponent->BindAction(_doubleTapDash_W_Action, ETriggerEvent::Triggered, this, &ATFT_Player::DoubleTapDash_Front);
		EnhancedInputComponent->BindAction(_doubleTapDash_A_Action, ETriggerEvent::Triggered, this, &ATFT_Player::DoubleTapDash_Left);
		EnhancedInputComponent->BindAction(_doubleTapDash_S_Action, ETriggerEvent::Triggered, this, &ATFT_Player::DoubleTapDash_Back);
		EnhancedInputComponent->BindAction(_doubleTapDash_D_Action, ETriggerEvent::Triggered, this, &ATFT_Player::DoubleTapDash_Right);

		EnhancedInputComponent->BindAction(_attackAction, ETriggerEvent::Started, this, &ATFT_Player::AttackA);

		EnhancedInputComponent->BindAction(_invenOpenAction, ETriggerEvent::Started, this, &ATFT_Player::InvenopenA);

		EnhancedInputComponent->BindAction(_EquipmentAction, ETriggerEvent::Started, this, &ATFT_Player::EquipmentA);

		EnhancedInputComponent->BindAction(_ESkillAction, ETriggerEvent::Started, this, &ATFT_Player::E_Skill);

		EnhancedInputComponent->BindAction(_QSkillAction, ETriggerEvent::Started, this, &ATFT_Player::Q_Skill);

		EnhancedInputComponent->BindAction(_rollingAction, ETriggerEvent::Started, this, &ATFT_Player::Rolling);

		EnhancedInputComponent->BindAction(_runningAction, ETriggerEvent::Started, this, &ATFT_Player::StartRunning);
		EnhancedInputComponent->BindAction(_runningAction, ETriggerEvent::Completed, this, &ATFT_Player::StopRunning);
	}
}

void ATFT_Player::SetMesh(FString path)
{
	_meshCom->SetMesh(path);
}

void ATFT_Player::Move(const FInputActionValue& value)
{
	if (!_canMove) return;

	if (GetCurHp() <= 0) return;

	FVector2D MovementVector = value.Get<FVector2D>();

	if (bBlockInputOnDash) return;

	if (Controller != nullptr)
	{
		_vertical = MovementVector.Y;
		_horizontal = MovementVector.X;

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATFT_Player::Look(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	FVector2D LookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void ATFT_Player::JumpA(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	bool isPressed = value.Get<bool>();

	if (bBlockInputOnDash) return;

	if (isPressed)
	{
		ACharacter::Jump();
	}
}

void ATFT_Player::AttackA(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;
	if (_invenCom->_currentWeapon == nullptr) return;

	bool isPressed = value.Get<bool>();

	//if (_isAttacking == false && isPressed && _animInstanceTM != nullptr)
	//{
	//	if (auto _animInstTM = Cast<UTFT_AnimInstance_TestMannequin>(_animInstanceTM))
	//	{
	//		if (_invenCom->_currentWeapon->_Itemid == 1)
	//		{
	//			_animInstTM->PlayAttackMontage();
	//			_isAttacking = true;

	//			_curAttackIndex %= 3;
	//			_curAttackIndex++;

	//			_animInstTM->JumpToSection(_curAttackIndex);
	//		}
	//		else if (_invenCom->_currentWeapon->_Itemid == 3)
	//		{
	//			_animInstTM->PlayAttackMontage2Hend();
	//			_isAttacking = true;

	//			_curAttackIndex %= 2;
	//			_curAttackIndex++;

	//			_animInstTM->JumpToSection(_curAttackIndex);
	//		}
	//		else
	//		{
	//			//UE_LOG(LogTemp, Log, TEXT("no Weapon no attack"));
	//			_animInstTM->PlayAttackMontage();
	//		}
	//	}
	//}
}

void ATFT_Player::InvenopenA(const FInputActionValue& value)
{
	UIMANAGER->GetInvenUI()->UIGold(_invenCom->GetPlayerGold());

	UIMANAGER->_invenOpenEvent.Broadcast();
}

void ATFT_Player::EquipmentA(const FInputActionValue& value)
{
	UIMANAGER->_EquipmentOpenEvent.Broadcast();
}

void ATFT_Player::Rolling()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(_rollingMontage);
}

void ATFT_Player::E_Skill(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	bool isPressed = value.Get<bool>();

	/*if (_invenCom->_currentWeapon == nullptr) return;

	if (isPressed && _animInstanceTM != nullptr && _invenCom->_currentWeapon->_Itemid == 3)
	{
		if (auto _animInstTM = Cast<UTFT_AnimInstance_TestMannequin>(_animInstanceTM))
		{
			_animInstTM->PlayE_SkillMontage();


			UIMANAGER->GetSkillUI()->RunCDT(1);
		}
	}*/
}

void ATFT_Player::Q_Skill(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	bool isPressed = value.Get<bool>();

	/*if (_invenCom->_currentWeapon == nullptr) return;

	if (isPressed && _animInstanceTM != nullptr && _invenCom->_currentWeapon->_Itemid == 1)
	{
		if (auto _animInstTM = Cast<UTFT_AnimInstance_TestMannequin>(_animInstanceTM))
		{
			_animInstTM->PlayQ_SkillMontage();


			UIMANAGER->GetSkillUI()->RunCDT(0);
		}
	}*/
}

void ATFT_Player::DoubleTapDash_Front(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		isDashing = true;
		bBlockInputOnDash = true;
		bCanDash = false;

		FVector from = GetActorLocation();
		FVector to = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
		to.Z = from.Z;

		FVector _dir = UKismetMathLibrary::GetDirectionUnitVector(from, to).RotateAngleAxis(180.0f, FVector(0, 0, 1));

		LaunchCharacter((_dir * dashStrength_Ground), false, false);

		FTimerHandle _timerHandle;
		GetWorldTimerManager().SetTimer(_timerHandle, this, &ATFT_Player::SetBlockInputOnDash_False, 0.5f, false);
	}
}

void ATFT_Player::DoubleTapDash_Back(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		isDashing = true;
		bBlockInputOnDash = true;
		bCanDash = false;

		FVector from = GetActorLocation();
		FVector to = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
		to.Z = from.Z;

		FVector _dir = UKismetMathLibrary::GetDirectionUnitVector(from, to);

		LaunchCharacter((_dir * dashStrength_Ground), false, false);

		FTimerHandle _timerHandle;
		GetWorldTimerManager().SetTimer(_timerHandle, this, &ATFT_Player::SetBlockInputOnDash_False, 0.5f, false);
	}
}

void ATFT_Player::DoubleTapDash_Left(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		isDashing = true;
		bBlockInputOnDash = true;
		bCanDash = false;

		FVector from = GetActorLocation();
		FVector to = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
		to.Z = from.Z;

		FVector _dir = UKismetMathLibrary::GetDirectionUnitVector(from, to).RotateAngleAxis(90.0f, FVector(0, 0, 1));

		LaunchCharacter((_dir * dashStrength_Ground), false, false);

		FTimerHandle _timerHandle;
		GetWorldTimerManager().SetTimer(_timerHandle, this, &ATFT_Player::SetBlockInputOnDash_False, 0.5f, false);
	}
}

void ATFT_Player::DoubleTapDash_Right(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		isDashing = true;
		bBlockInputOnDash = true;
		bCanDash = false;

		FVector from = GetActorLocation();
		FVector to = GetWorld()->GetFirstPlayerController()->GetPawn()->FindComponentByClass<UCameraComponent>()->GetComponentLocation();
		to.Z = from.Z;

		FVector _dir = UKismetMathLibrary::GetDirectionUnitVector(from, to).RotateAngleAxis(270.0f, FVector(0, 0, 1));

		LaunchCharacter((_dir * dashStrength_Ground), false, false);

		FTimerHandle _timerHandle;
		GetWorldTimerManager().SetTimer(_timerHandle, this, &ATFT_Player::SetBlockInputOnDash_False, 0.5f, false);
	}
}

void ATFT_Player::DashEnd()
{
	if (!bCanDash)
	{
		bCanDash = true;
		bBlockInputOnDash = false;

		GetCharacterMovement()->StopMovementImmediately();

		isDashing = false;
	}
}

void ATFT_Player::StartRunning()
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = runningSpeed;

	/*if (_animInstanceTM)
	{
		_animInstanceTM->PlaySprintMontage();
	}*/
}

void ATFT_Player::StopRunning()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	/*if (_animInstanceTM)
	{
		_animInstanceTM->StopSprintMontage();
	}*/
}

void ATFT_Player::AttackStart()
{
	/*if (_invenCom->_currentWeapon->_Itemid == 1)
	{
		SoundManager->Play("Knight_Swing", GetActorLocation());
	}
	else if (_invenCom->_currentWeapon->_Itemid == 3)
	{
		SoundManager->Play("Hammer_Swing", GetActorLocation());
	}*/
}

void ATFT_Player::AttackHit()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 500.0f;
	float attackRadius = 100.0f;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(attackRadius),
		params
	);

	FVector vec = GetActorForwardVector() * attackRange;
	FVector center = GetActorLocation() + vec * 0.5f;
	FColor drawColor = FColor::Green;

	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		FDamageEvent damageEvent;

		hitResult.GetActor()->TakeDamage(50.0f, damageEvent, GetController(), this);
		_hitPoint = hitResult.ImpactPoint;
	}

	// DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_Player::AttackHit_Q()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 500.0f;
	float attackRadius = 100.0f;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(attackRadius),
		params
	);

	FVector vec = GetActorForwardVector() * attackRange;
	FVector center = GetActorLocation() + vec * 0.5f;
	FColor drawColor = FColor::Green;

	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		FDamageEvent damageEvent;

		hitResult.GetActor()->TakeDamage(300.0f, damageEvent, GetController(), this);
		_hitPoint = hitResult.ImpactPoint;
	}

	// DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_Player::AddItemPlayer(ATFT_Item* item)
{
	if (_invenCom != nullptr) _invenCom->AddItem(item);
}

void ATFT_Player::AddItemHendle(ATFT_Item* item, int32 index)
{
	UIMANAGER->GetInvenUI()->AddUiItem(item, index);
}

void ATFT_Player::DropItemPlayer(ATFT_Item* item, int32 index)
{
	if (_invenCom != nullptr) _invenCom->DropItem(index);
}

void ATFT_Player::SellItemPlayer(ATFT_Item* item, int32 index)
{
	if (_invenCom != nullptr)  _invenCom->SellItem(index);
}

void ATFT_Player::UseItemPlayer(ATFT_Item* item, int32 index)
{
	if (item->GetItemType() == "Equipment")
	{
		UE_LOG(LogTemp, Log, TEXT("Equipment Item Use"));

		UIMANAGER->GetEquipmentUI()->Set_Equipment(item);
		_invenCom->UseItem(index);
	}
	else if (item->GetItemType() == "Utility")
	{
		UE_LOG(LogTemp, Log, TEXT("Utility Item Use"));

		_invenCom->UseItem(index);
	}
}

void ATFT_Player::UseItemPlayer_Equipment(ATFT_Item* item)
{
	if (_statCom != nullptr) _statCom->AddAttackDamage(item->GetItemAttackDamage());

}

void ATFT_Player::UIGold(int32 gold)
{
	UIMANAGER->GetInvenUI()->UIGold(gold);

}

void ATFT_Player::ChangeEquipment(ATFT_Item* item)
{
	if (_statCom != nullptr) _statCom->AddAttackDamage(-item->GetItemAttackDamage());

	_invenCom->AddItem(item);
}

void ATFT_Player::CloseResetEquipment()
{
	UIMANAGER->GetEquipmentUI()->ResetChoice();
}