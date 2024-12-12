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
		_attackSpeed = data->attackSpeed;
		_skillDamage = data->skillDamage;

		if (AddsItem_Attack != 0) _attackDamage += AddsItem_Attack;

		UE_LOG(LogTemp,Error, TEXT("%s..., Level : %d, hp : %d, attackDamage : %d, skillDamage : %d"), *GetOwner()->GetName(), _curLevel, _maxHp, _attackDamage, _skillDamage);
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
	if (_curHp > _maxHp) _curHp = _maxHp;


	//float ratio = HpRatio();//
	//_hpChangedDelegate.Broadcast(ratio);//

	// float bossratio = 0.15 + BossHPRatio();
	float bossratio = BossHPRatio();
	_BosshpChangedDelegate.Broadcast(bossratio);

	// float playerratio = 0.25 + (HpRatio() * (0.7 - 0.25));
	float playerratio = HpRatio();
	_PlayerhpChangedDelegate.Broadcast(playerratio);

	//float HPRatio = static_cast<float>(_curHp) / _maxHp;//
	//OnHPChanged.Broadcast(HPRatio);//
}

void UTFT_StatComponent::SetBarrier(int32 barrier)
{
	_curBarrier = barrier;
	if (_curBarrier < 0) _curBarrier = 0;
	if (_curBarrier > _maxBarrier) _curBarrier = _maxBarrier;

	float Bratio = BarrierRatio();
	_PlayerBarrierChangedDelegate.Broadcast(Bratio);
	_CurBarrierText.Broadcast(_curBarrier);

}

int32 UTFT_StatComponent::AddCurHp(float amount)
{
	int32 remainB = 0;
	if (_curBarrier > 0 && (amount < 0)) // ����� 1�̶� �ְ� �������϶���
	{
		int32 b = AddCurBarrier(amount);
		
		if (b == -7249)
		{
			remainB = _curBarrier;
			_curBarrier = 0;
			SetBarrier(_curBarrier);
		}
		else return 0;
	} //����� ���� ���¿����� hp������ �״�� ����

	int32 beforeHp = _curHp + remainB;

	int32 afterHp = beforeHp + amount;

	if (afterHp >= _maxHp) afterHp = _maxHp;

	_CurHpText.Broadcast(afterHp);
	SetHp(afterHp);

	return afterHp - beforeHp;
}

int32 UTFT_StatComponent::AddCurBarrier(float amount)
{
	if ((_curBarrier + amount) < 0) return -7249;
	
	int32 beforeB = _curBarrier;
	int32 afterB = beforeB + amount;

	_CurBarrierText.Broadcast(afterB);
	SetBarrier(afterB);


	return afterB - beforeB;
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

void UTFT_StatComponent::AddExp(int32 amount)
{
	_curExp += amount;

	_CurExpText.Broadcast(_curExp);

	if (_curLevel >= _maxLevel) return;

	if (_curExp >= _maxExp)
	{
		_curExp -= _maxExp;

		_curLevel++;
		_levelUpDelegate.Broadcast(); // Level up effect

		SetLevelAndInit(_curLevel);
	}

	float ratio = ExpRatio();

	_expChangedDelegate.Broadcast(ratio);
}
