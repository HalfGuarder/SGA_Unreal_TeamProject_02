// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Player.h"

#include "TFT_Creature.h"
#include "TFT_Monster.h"
#include "TFT_Widgets/TFT_HPBarWidget.h"

#include "TFT_AnimInstance_Player.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

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
#include "Engine/OverlapResult.h"

ATFT_Player::ATFT_Player()
{
	_meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));

	_invenCom = CreateDefaultSubobject<UTFT_InvenComponent>(TEXT("Inven_Com"));

	SetMesh("/Script/Engine.SkeletalMesh'/Game/Blueprints/Characters/Player/Mesh/TFT_SK_SciFi_Soldier_Female_Skin1.TFT_SK_SciFi_Soldier_Female_Skin1'");

	_cameraTLCom = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTLComponent"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> cf
	(TEXT("/Script/Engine.CurveFloat'/Game/Blueprints/Characters/Player/Camera/TFT_Player_Camera_Zoom_Curve.TFT_Player_Camera_Zoom_Curve'"));
	if(cf.Succeeded())
	{
		_cameraZoomCurve = cf.Object;
	}

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArm->SetupAttachment(GetCapsuleComponent());
	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));

	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_camera->SetupAttachment(_springArm);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	_statCom->SetExp(0);

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Widget/Player_HpBar_BP.Player_HpBar_BP_C'"));
	if (HpBar.Succeeded())
	{
		HpBarWidgetClass = HpBar.Class;
	}

	if (HpBarWidgetClass)
	{
		HpBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HpBarWidgetClass);
		if (HpBarWidgetInstance)
		{
			HpBarWidgetInstance->AddToViewport();
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> sm
	(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Blueprints/Characters/Player/FX/M_Steel_Armor_ShieldSphere_Impact_Inst.M_Steel_Armor_ShieldSphere_Impact_Inst'"));
	if (sm.Succeeded())
	{
		_shieldMaterial = sm.Object;
	}
	_shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	_shield->SetupAttachment(GetCapsuleComponent());
	_shield->SetMaterial(0, _shieldMaterial);
	_shield->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	_shield->SetVisibility(false);

	_shieldDashAttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldDashAttackSphere"));
	_shieldDashAttackSphere->SetupAttachment(GetCapsuleComponent());
	_shieldDashAttackSphere->SetupAttachment(_shield);
	_shieldDashAttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATFT_Player::BeginPlay()
{
	Super::BeginPlay();

	_statCom->SetLevelAndInit(1);

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

	if (_shieldDashAttackSphere != nullptr)
	{
		_shieldDashAttackSphere->OnComponentBeginOverlap.AddDynamic(this, &ATFT_Player::ShieldDash_OnOverlapBegin);
	}

	_cameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
	_cameraTLCom->AddInterpFloat(_cameraZoomCurve, _cameraZoomHandler);
}

void ATFT_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_animInstancePlayer = Cast<UTFT_AnimInstance_Player>(GetMesh()->GetAnimInstance());
	if (_animInstancePlayer->IsValidLowLevel())
	{
		_animInstancePlayer->_dashEndDelegate.AddUObject(this, &ATFT_Player::DashEnd);
		_animInstancePlayer->OnMontageEnded.AddDynamic(this, &ATFT_Creature::OnAttackEnded);
		_animInstancePlayer->_shieldDashEndDelegate.AddUObject(this, &ATFT_Player::StopShieldDash);
		_animInstancePlayer->_attackHitDelegate.AddUObject(this, &ATFT_Player::AttackHit);
		_animInstancePlayer->_qSkillHitDelegate.AddUObject(this, &ATFT_Player::Q_SkillHit);
		_animInstancePlayer->_eSkillHitDelegate.AddUObject(this, &ATFT_Player::E_SkillHit);

	}
	

	if (HpBarWidgetInstance)
	{

		UTFT_HPBarWidget* HpBar = Cast<UTFT_HPBarWidget>(HpBarWidgetInstance);
		if (HpBar)
		{
			HpBar->SetProfileImage(ProfileType::PLAYER);
			HpBar->SetHpText(_statCom->GetMaxHp());
			_statCom->_PlayerhpChangedDelegate.AddUObject(HpBar, &UTFT_HPBarWidget::SetHpBarValue);
			_statCom->_CurHpText.AddUObject(HpBar, &UTFT_HPBarWidget::CurHpText);
		}
	}
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

		EnhancedInputComponent->BindAction(_defenseAction, ETriggerEvent::Started, this, &ATFT_Player::StartDefense);
		EnhancedInputComponent->BindAction(_defenseAction, ETriggerEvent::Started, this, &ATFT_Player::OnShield);
		EnhancedInputComponent->BindAction(_defenseAction, ETriggerEvent::Completed, this, &ATFT_Player::StopDefense);
		EnhancedInputComponent->BindAction(_defenseAction, ETriggerEvent::Completed, this, &ATFT_Player::OffShield);

		EnhancedInputComponent->BindAction(_tempAction, ETriggerEvent::Started, this, &ATFT_Player::Temp_ChangeWeapon);
	}
}

void ATFT_Player::SetMesh(FString path)
{
	_meshCom->SetMesh(path);
}

void ATFT_Player::Temp_ChangeWeapon(const FInputActionValue& value)
{
	if (bIsZoom) return;

	bool isPressed = value.Get<bool>();

	if (isPressed)
	{
		if (bEquipSword)
		{
			bEquipSword = false;
		}
		else
		{
			bEquipSword = true;
		}
	}
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

		if (bIsRunning)
		{
			if (_vertical < 0)
			{
				StopRunning();
				
				return;
			}

			if (_horizontal != 0)
			{
				AddMovementInput(GetActorForwardVector(), MovementVector.Y);

				return;
			}
		}

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATFT_Player::Look(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	FVector2D LookAxisVector = value.Get<FVector2D>();

	if (!_canMove)
	{
		bUseControllerRotationYaw = false;
	}

	if (Controller != nullptr)
	{
		AddControllerPitchInput(-LookAxisVector.Y);


		AddControllerYawInput(LookAxisVector.X);
	}
}

void ATFT_Player::JumpA(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	if (!_canMove) return;

	if (bIsRunning) return;

	if (bBlockInputOnDash) return;

	bool isPressed = value.Get<bool>();

	if (isPressed)
	{
		ACharacter::Jump();
	}
}

void ATFT_Player::AttackA(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;
	// if (_invenCom->_currentWeapon == nullptr) return;

	bool isPressed = value.Get<bool>();

	if (_isAttacking == false && isPressed && _animInstancePlayer != nullptr)
	{
		// _animInstancePlayer->StopRunningMontage();
		_animInstancePlayer->PlayAttackMontage();
		_isAttacking = true;
		if(bEquipSword) _canMove = false;
		_curAttackIndex %= 3;
		_curAttackIndex++;
		_animInstancePlayer->JumpToSection(_curAttackIndex);
		
		//if (auto _animInstTM = Cast<UTFT_AnimInstance_Player>(_animInstancePlayer))
		//{
		//	if (_invenCom->_currentWeapon->_Itemid == 1)
		//	{
		//		_animInstTM->PlayAttackMontage();
		//		_isAttacking = true;

		//		_curAttackIndex %= 3;
		//		_curAttackIndex++;

		//		_animInstTM->JumpToSection(_curAttackIndex);
		//	}
		//	else if (_invenCom->_currentWeapon->_Itemid == 3)
		//	{
		//		_animInstTM->PlayAttackMontage2Hend();
		//		_isAttacking = true;

		//		_curAttackIndex %= 2;
		//		_curAttackIndex++;

		//		_animInstTM->JumpToSection(_curAttackIndex);
		//	}
		//	else
		//	{
		//		//UE_LOG(LogTemp, Log, TEXT("no Weapon no attack"));
		//		_animInstTM->PlayAttackMontage();
		//	}
		//}
	}
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

void ATFT_Player::CameraZoom(float alpha)
{
	_springArm->TargetArmLength = FMath::Lerp(_defalutSpringArmLength, _zoomSpringArmLength, alpha);
}

void ATFT_Player::E_Skill(const FInputActionValue& value)
{
	if (GetCurHp() <= 0) return;

	if (!bIsDefense) return;

	bool isPressed = value.Get<bool>();

	StopDefense();

	_animInstancePlayer->PlayUpperSwingMontage();

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

	if (!bIsDefense) return;

	bool isPressed = value.Get<bool>();

	bIsShieldDashing = true;

	StopDefense();

	_animInstancePlayer->PlayShieldDashMontage();

	_shieldDashAttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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

	if (bIsRunning) return;

	if (!_canMove) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		bIsDashing = true;
		bFDashing = true;
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

	if (bIsRunning) return;

	if (!_canMove) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		bIsDashing = true;
		bBDashing = true;
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

	if (bIsRunning) return;

	if (!_canMove) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		bIsDashing = true;
		bLDashing = true;
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

	if (bIsRunning) return;

	if (!_canMove) return;

	if (bCanDash && !GetCharacterMovement()->IsFalling())
	{
		bIsDashing = true;
		bRDashing = true;
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

		bFDashing = false;
		bBDashing = false;
		bLDashing = false;
		bRDashing = false;	

		bIsDashing = false;
	}
}

void ATFT_Player::StartRunning()
{
	if (!_canMove) return;

	if (bIsDefense || bIsZoom) return;

	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = runningSpeed;

	if (_animInstancePlayer)
	{
		_animInstancePlayer->PlayRunningMontage();
	}
}

void ATFT_Player::StopRunning()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	if (_animInstancePlayer)
	{
		_animInstancePlayer->StopRunningMontage();
	}
}

void ATFT_Player::StartDefense()
{
	if (bIsShieldDashing) return;

	bIsDefense = true;
	GetCharacterMovement()->MaxWalkSpeed = defenseWalkSpeed;

	if (bEquipSword)
	{
		if (_animInstancePlayer)
		{
			EFFECTMANAGER->Play(TEXT("ShieldOn"), 0, GetActorLocation());
			_animInstancePlayer->bIsDefensing = true;
			_animInstancePlayer->PlayDefenseMontage();
		}
	}
	else
	{
		_cameraTLCom->Play();
		bIsZoom = true;
	}
}

void ATFT_Player::StopDefense()
{
	bIsDefense = false;
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	if (bEquipSword)
	{
		if (_animInstancePlayer)
		{
			_animInstancePlayer->bIsDefensing = false;
			_animInstancePlayer->StopDefenseMontage();
		}
	}
	else
	{
		_cameraTLCom->Reverse();
		bIsZoom = false;
	}
}

void ATFT_Player::StopShieldDash()
{
	bIsShieldDashing = false;

	OffShield();
}

void ATFT_Player::OffShield()
{
	if (bIsShieldDashing) return;

	_shield->SetVisibility(false);
}

void ATFT_Player::OnShield()
{
	_shield->SetVisibility(true);
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
		ECollisionChannel::ECC_GameTraceChannel7,
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

		hitResult.GetActor()->TakeDamage(_statCom->GetAttackDamage(), damageEvent, GetController(), this);
		_hitPoint = hitResult.ImpactPoint;
	}

	DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 2.0f);
}

void ATFT_Player::Q_SkillHit()
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
		ECollisionChannel::ECC_GameTraceChannel7,
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

	DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 0.1f);
}

void ATFT_Player::E_SkillHit()
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
		ECollisionChannel::ECC_GameTraceChannel7,
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

	DrawDebugSphere(GetWorld(), center, attackRadius, 20, drawColor, false, 0.1f);
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

void ATFT_Player::ShieldDash_OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto enemy = Cast<ATFT_Monster>(OtherActor);

	if (enemy != nullptr)
	{
		Q_SkillHit();

		_shieldDashAttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
