// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"

#include "TFT_StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(DeathDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(HpChanged, float)
DECLARE_MULTICAST_DELEGATE_OneParam(ExpChanged, float)
DECLARE_MULTICAST_DELEGATE(LevelUp)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDelegate, float)
DECLARE_MULTICAST_DELEGATE_OneParam(CurHpText, int32)


USTRUCT()
struct FTFT_StatData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxHP = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxMP = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxExp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 attack = 0;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFT_PROJECT_B_API UTFT_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTFT_StatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Reset();


	int32 GetCurHp() { return _curHp; }
	int32 GetMaxHp() { return _maxHp; }
	int32 GetCurMp() { return _curMp; }
	int32 GetMaxMp() { return _maxMp; }
	int32 GetCurExp() { return _curExp; }
	int32 GetMaxExp() { return _maxExp; }
	int32 GetAttackDamage() { return _attackDamage; }
	int32 GetCurBarrier() { return _curBarrier; }
	float HpRatio() { return _curHp / (float)_maxHp; }
	float ExpRatio() { return _curExp / (float)_maxExp; }
	float BossHPRatio() { return (_curHp / (float)_maxHp) / 2; }
	float BarrierRatio() { return _curBarrier / (float)_maxBarrier; }

	void SetLevelAndInit(int32 level);
	void SetHp(int32 hp);
	void SetBarrier(int32 barrier);

	int32 AddCurHp(float amount);
	int32 AddCurBarrier(float amount);
	void AddAttackDamage(float amount);
	void AddMaxHp(int32 amount);

	bool IsDead() { return _curHp <= 0; }

	void SetExp(int32 amount);

	HpChanged _hpChangedDelegate;
	HpChanged _BosshpChangedDelegate;
	HpChanged _PlayerhpChangedDelegate;
	CurHpText _CurHpText;
	DeathDelegate _deathDelegate;
	ExpChanged _expChangedDelegate;
	LevelUp _levelUpDelegate;
	CurHpText _CurBarrierText;
	HpChanged _PlayerBarrierChangedDelegate;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _curLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _maxLevel = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _maxHp = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _curHp = _maxHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _maxMp = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _curMp = _maxMp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _curExp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _maxExp = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _attackDamage = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _defense = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _curBarrier = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 _maxBarrier = 100;

private:
	int32 AddsItem_HP = 0;
	int32 AddsItem_Attack = 0;
};
