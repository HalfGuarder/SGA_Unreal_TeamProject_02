// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TFT_Creature.h"
#include "TFT_Player.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UBoxComponent;

UCLASS()
class TFT_PROJECT_B_API ATFT_Player : public ATFT_Creature
{
	GENERATED_BODY()

public:
	ATFT_Player();

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Init();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// virtual void Interact(ATFT_Item* item) override;

	virtual void SetMesh(FString path) override;

protected:
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void JumpA(const FInputActionValue& value);
	void AttackA(const FInputActionValue& value);
	void InvenopenA(const FInputActionValue& value);
	void EquipmentA(const FInputActionValue& value);
	void Rolling();

	void E_Skill(const FInputActionValue& value);
	void Q_Skill(const FInputActionValue& value);

	void DoubleTapDash_Front(const FInputActionValue& value);
	void DoubleTapDash_Back(const FInputActionValue& value);
	void DoubleTapDash_Left(const FInputActionValue& value);
	void DoubleTapDash_Right(const FInputActionValue& value);
	void DashEnd();
	void SetBlockInputOnDash_False() { bBlockInputOnDash = false; }
	void StartRunning();
	void StopRunning();

	virtual void AttackStart() override;
	UFUNCTION()
	void AttackHit();
	UFUNCTION()
	void AttackHit_Q();

public:
	void AddItemPlayer(ATFT_Item* item);
	void AddItemHendle(ATFT_Item* item, int32 index);
	void DropItemPlayer(ATFT_Item* item, int32 index);
	void SellItemPlayer(ATFT_Item* item, int32 index);
	void UseItemPlayer(ATFT_Item* item, int32 index);
	void UseItemPlayer_Equipment(ATFT_Item* item);
	void UIGold(int32 gold);
	void ChangeEquipment(ATFT_Item* item);
	void CloseResetEquipment();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _jumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AlllowPrivateAccess = "true"))
	bool _canMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AlllowPrivateAccess = "true"))
	UInputAction* _doubleTapDash_W_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AlllowPrivateAccess = "true"))
	UInputAction* _doubleTapDash_A_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AlllowPrivateAccess = "true"))
	UInputAction* _doubleTapDash_S_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AlllowPrivateAccess = "true"))
	UInputAction* _doubleTapDash_D_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _invenOpenAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _EquipmentAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _interactAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _ESkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _QSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _runningAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* _rollingAction;

	class ITFT_InteractionInterface* _interface = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _attackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _springArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _camera;

	UPROPERTY(EditAnywhere)
	UAnimMontage* _rollingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* _myAnimMontage;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bCanDash = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bBlockInputOnDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float dashStrength_Ground = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float dashStrength_Air = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta =(UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0"))
	float dashEndDempener;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UBoxComponent* _triggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float runningSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float walkSpeed = 600.0f;
};
