// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_StatComponent.h"

#include "TFT_GameInstance.h"

UTFT_StatComponent::UTFT_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTFT_StatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTFT_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTFT_StatComponent::Reset()
{
	_curHp = _maxHp;
	_curMp = _maxMp;
}

void UTFT_StatComponent::SetLevelAndInit(int32 level)
{
	auto myGameInstance = Cast<UTFT_GameInstance>(GetWorld()->GetGameInstance());
	if (myGameInstance)
	{
		FTFT_StatData* data = myGameInstance->GetStatDataByLevel(level);
		_curLevel = level;
		_maxHp = data->maxHP;
		_curHp = _maxHp;
		_maxMp = data->maxMP;
		_curMp = _maxMp;
		_maxExp = data->maxExp;
		_attackDamage = data->attack;

		if (AddsItem_Attack != 0) _attackDamage += AddsItem_Attack;

		UE_LOG(LogTemp, Warning, TEXT("%s..., Level : %d, hp : %d, attackDamage : %d"), *GetOwner()->GetName(), _curLevel, _maxHp, _attackDamage);
	}
}

void UTFT_StatComponent::SetHp(int32 hp)
{
	_curHp = hp;
	if (_curHp <= 0)
	{
		_deathDelegate.Broadcast();
		_curHp = 0;
	}
	if (_curHp > _maxHp)
		_curHp = _maxHp;

	float ratio = HpRatio();
	_hpChangedDelegate.Broadcast(ratio);

	float HPRatio = static_cast<float>(_curHp) / _maxHp;
	OnHPChanged.Broadcast(HPRatio);
}

int32 UTFT_StatComponent::AddCurHp(float amount)
{
	int32 beforeHp = _curHp;

	int32 afterHp = beforeHp + amount;
	SetHp(afterHp);

	return afterHp - beforeHp;
}

void UTFT_StatComponent::AddAttackDamage(float amount)
{
	_attackDamage += amount;
	AddsItem_Attack += amount;
}

void UTFT_StatComponent::AddMaxHp(int32 amount)
{
	_maxHp += amount;
	AddsItem_HP += amount;
}

void UTFT_StatComponent::SetExp(int32 amount)
{
	_curExp += amount;

	if (_curExp >= _maxExp)
	{
		_curExp = _curExp - _maxExp;

		_curLevel++;
		if (_curLevel >= _maxLevel) _curLevel = _maxLevel;
		_levelUpDelegate.Broadcast();

		SetLevelAndInit(_curLevel);
	}

	float ratio = ExpRatio();

	_expChangedDelegate.Broadcast(ratio);
}
