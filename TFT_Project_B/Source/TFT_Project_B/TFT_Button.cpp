// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Button.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TFT_GameInstance.h"
#include "TFT_Managers/TFT_EffectManager.h"
#include "Engine/DamageEvents.h"
#include "TFT_Characters/TFT_Monster.h"
#include "TFT_Characters/TFT_Player.h"

ATFT_Button::ATFT_Button()
{
	PrimaryActorTick.bCanEverTick = true;

	// BoxComponent 초기화
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	ButtonMesh->SetupAttachment(RootComponent);

	EffectCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EffectCollision"));
	EffectCollision->SetupAttachment(RootComponent);
	EffectCollision->SetSphereRadius(1000.0f);
	EffectCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EffectCollision->OnComponentBeginOverlap.AddDynamic(this, &ATFT_Button::OnEffectOverlap);

	bIsOn = false;
	bIsCooldown = false;
	bBossInCollision = false;
	bPlayerNearby = false;
}

void ATFT_Button::BeginPlay()
{
	Super::BeginPlay();
}

void ATFT_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATFT_Button::ToggleButton()
{
	if (!bPlayerNearby) // 플레이어가 버튼 근처에 없으면 동작하지 않음
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is not near the button!"));
		return;
	}

	if (bIsCooldown) // 쿨다운 중인 경우에도 동작하지 않음
	{
		UE_LOG(LogTemp, Warning, TEXT("Button is on cooldown!"));
		return;
	}

	bIsOn = true;
	ActivateEnergyEffect(); // 에너지 이펙트 활성화
	StartCooldown();        // 쿨다운 시작
}

void ATFT_Button::ActivateEnergyEffect()
{
	if (EFFECTMANAGER)
	{
		FVector EffectLocation = GetActorLocation() + EffectLocationOffset;
		FRotator EffectRotation = GetActorRotation() + EffectRotationOffset;

		EFFECTMANAGER->Play(TEXT("Energy"), 1, EffectLocation, EffectRotation);
		UE_LOG(LogTemp, Log, TEXT("Energy Effect Activated!"));

		// 5초 후 AttackUp 이펙트 활성화
		GetWorldTimerManager().SetTimer(EnergyEffectTimerHandle, this, &ATFT_Button::ActivateAttackUpEffect, 5.0f, false);
	}
}

void ATFT_Button::ActivateAttackUpEffect()
{
	if (EFFECTMANAGER)
	{
		FVector EffectLocation = GetActorLocation() + EffectLocationOffset;
		FRotator EffectRotation = GetActorRotation() + EffectRotationOffset;

		EFFECTMANAGER->Play(TEXT("ATTACKUP"), 1, EffectLocation, EffectRotation);
		UE_LOG(LogTemp, Log, TEXT("AttackUp Effect Activated!"));

		// 콜리전 활성화
		EnableEffectCollision();
	}
}

void ATFT_Button::EnableEffectCollision()
{
	EffectCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bBossInCollision = false;

	// 2초 후 콜리전 비활성화
	GetWorldTimerManager().SetTimer(AttackUpEffectTimerHandle, this, &ATFT_Button::DisableEffectCollision, 2.0f, false);
}

void ATFT_Button::DisableEffectCollision()
{
	EffectCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Log, TEXT("Effect Collision Disabled"));
}

void ATFT_Button::StartCooldown()
{
	bIsCooldown = true;
	UE_LOG(LogTemp, Log, TEXT("Cooldown started!"));

	// 10초 쿨다운 후 해제
	GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ATFT_Button::EndCooldown, 10.0f, false);
}

void ATFT_Button::EndCooldown()
{
	bIsCooldown = false;
	UE_LOG(LogTemp, Log, TEXT("Cooldown ended!"));
}

void ATFT_Button::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ATFT_Player::StaticClass())) // 버튼 근처에 들어온 Actor가 플레이어인지 확인
	{
		bPlayerNearby = true;
		UE_LOG(LogTemp, Log, TEXT("Player is near the button."));
	}
}

void ATFT_Button::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ATFT_Player::StaticClass())) // 버튼 범위를 벗어난 Actor가 플레이어인지 확인
	{
		bPlayerNearby = false;
		UE_LOG(LogTemp, Log, TEXT("Player left the button."));
	}
}

void ATFT_Button::OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATFT_Monster* Boss = Cast<ATFT_Monster>(OtherActor);
	if (Boss)
	{
		bBossInCollision = true;
		ApplyDamageToBoss();
	}
}

void ATFT_Button::ApplyDamageToBoss()
{
	if (bBossInCollision)
	{
		ATFT_Monster* Boss = Cast<ATFT_Monster>(UGameplayStatics::GetActorOfClass(GetWorld(), ATFT_Monster::StaticClass()));
		if (Boss)
		{
			// 2초간 초당 10의 데미지 적용
			for (int i = 0; i < 2; i++)
			{
				Boss->TakeDamage(10.0f, FDamageEvent(), nullptr, this);
				UE_LOG(LogTemp, Log, TEXT("Boss took 10 damage!"));
			}
		}
	}
}


