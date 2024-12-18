// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Monster.h"
#include "TFT_Player.h"
#include "Engine/DamageEvents.h"
#include "TFT_StatComponent.h"
#include "AIController.h"
#include "TFT_Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TFT_DamageIndicator.h"

ATFT_Monster::ATFT_Monster()
{
	_invenCom = CreateDefaultSubobject<UTFT_InvenComponent>(TEXT("Inven_Com"));
	_meshCom = CreateDefaultSubobject<UTFT_MeshComponent>(TEXT("Mesh_Com"));
}

void ATFT_Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATFT_Monster::BeginPlay()
{
	Super::BeginPlay();
    _controller = Cast<ATFT_Boss_AIController>(GetController());
}

void ATFT_Monster::Attack_AI()
{
}

void ATFT_Monster::AttackEnd()
{
}

void ATFT_Monster::DropItem(MonsterType type)
{
    if (bItemDroped) return;

	if (IsValid(this)) // �Ǵ� IsValid(GetWorld())
	{
		FVector monsterPos = GetActorLocation();
		_invenCom->DropMonsterItem(monsterPos, type);

        bItemDroped = true;
	}
	//_invenCom->DropMonsterItem(monsterPos);
}

float ATFT_Monster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (_statCom->IsDead())
	{
		auto player = Cast<ATFT_Player>(DamageCauser);

		if (player != nullptr)
		{
			player->_statCom->AddExp(_possessionExp);
		}
	}

	if (auto Player = Cast<ATFT_Player>(DamageCauser))
	{
		UTFT_DamageIndicator* DamageIndicatorWidget = CreateWidget<UTFT_DamageIndicator>(GetWorld(), LoadClass<UTFT_DamageIndicator>(nullptr, TEXT("/Game/Blueprints/Widget/TFT_DamageIndicator_BP.TFT_DamageIndicator_BP")));

		if (DamageIndicatorWidget)
		{
			DamageIndicatorWidget->AddToViewport();
			DamageIndicatorWidget->ShowDamageIndicator();
		}
	}

	return damage;
}

void ATFT_Monster::DeathStart()
{
    Super::DeathStart();

    auto player = Cast<ATFT_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (player && _controller)
    {
        _controller->UnPossess();

        return;
    }
}

void ATFT_Monster::TakeDamage_BP(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ATFT_Monster::ChangeMesh(TObjectPtr<USkeletalMesh> mesh)
{
	_meshCom->ChangeMesh(mesh);
    bAnimBind = false;
}

void ATFT_Monster::PreActive()
{
    _meshCom->Activate(true);
}

void ATFT_Monster::Active()
{
    bIsSpawned = true;

    _meshCom->Activate(true);

    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    auto player = Cast<ATFT_Player>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (player && _controller)
    {
        _controller->Possess(this);

        _controller->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), player);

        return;
    }

    _statCom->Reset();

    PrimaryActorTick.bCanEverTick = true;

    bItemDroped = false;
}

void ATFT_Monster::DeActive()
{
    bIsSpawned = false;

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    /*if (_controller)
    {
        _controller->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Tartget")), nullptr);
    } */

    PrimaryActorTick.bCanEverTick = false;

    _meshCom->Deactivate();
}

void ATFT_Monster::SetAnimInstanceBind()
{
}

void ATFT_Monster::ChangeLevel(int32 level)
{
    _statCom->SetLevelAndInit(level);
}

void ATFT_Monster::ReStartBT(AActor* target)
{
    if (_controller)
    {
       _controller->GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), target);
       _controller->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("ReStart")), true);
       _controller->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("ReStart")), false);
    }
}
