// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TFT_MeshComponent.h"
#include "TFT_StatComponent.h"
#include "TFT_InvenComponent.h"
#include "TFT_StateComponent.h"

#include "TFT_Creature.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class UTFT_MeshComponent;
class UTFT_StatComponent;
class UTFT_InvenComponent;
class UTFT_StateComponent;

DECLARE_MULTICAST_DELEGATE(Delegate_AttackEnded);

UCLASS()
class TFT_PROJECT_B_API ATFT_Creature : public ACharacter
{
	GENERATED_BODY()

public:
	ATFT_Creature();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnAttackEnded(class UAnimMontage* Montage, bool bInterrupted);

	int32 GetCurHp() { return _statCom->GetCurHp(); }

	virtual bool CheckState(StateType type) { return _stateCom->CheckState(type); }
	virtual void SetState(StateType type) { _stateCom->SetState(type); }
	virtual void InitState() { _stateCom->InitState(); }

	virtual void SetMesh(FString path) PURE_VIRTUAL(ATFT_Creature::SetMesh, );

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	virtual void AttackStart();

	UFUNCTION()
	virtual void DeathStart();

	UFUNCTION()
	virtual void Disable();

	UFUNCTION()
	virtual void FootStep();

	virtual void StateCheck();

	UFUNCTION()
	virtual void EndState();

	Delegate_AttackEnded _attackEndedDelegate;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = true))
	UTFT_MeshComponent* _meshCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	UTFT_StatComponent* _statCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inven, meta = (AllowPrivateAccess = true))
	UTFT_InvenComponent* _invenCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inven, meta = (AllowPrivateAccess = "true"))
	UTFT_StateComponent* _stateCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = true))
	class UWidgetComponent* _hpbarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> HpBarWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shield, meta = (AllowPrivateAccess = "true"))
	bool bIsDefense = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool _isAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool _isUsingSkill = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	int _curAttackIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float _vertical = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float _horizontal = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AlllowPrivateAccess = "true"))
	bool _canMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = true))
	class UUserWidget* HpBarWidgetInstance;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Store, meta = (AllowPrivateAccess = true))
	// class UTFT_StoreComponent* _storeCom;

	// UTFT_StoreComponent* GetStoreCom() { return _storeCom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AttackHit, meta = (AllowPrivateAccess = "true"))
	FVector _hitPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bFDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bBDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bLDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dash, Meta = (AllowPrivateAccess = true))
	bool bRDashing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip, Meta = (AllowPrivateAccess = true))
	bool bEquipSword = false;

	// State
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = "true"))
	bool bIsOnState = false;
};
