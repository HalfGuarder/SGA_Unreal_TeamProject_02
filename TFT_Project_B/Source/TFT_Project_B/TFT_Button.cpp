// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Button.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TFT_Characters/TFT_Player.h"
#include "TFT_Managers/TFT_EffectManager.h"
#include "TFT_GameInstance.h"
#include "Components/SphereComponent.h"

// Sets default values
ATFT_Button::ATFT_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 콜리전 컴포넌트 생성
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATFT_Button::OnOverlapBegin);
    BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

    // 버튼 메시 생성s
    ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
    ButtonMesh->SetupAttachment(RootComponent);

    // EffectCollision 초기화 - 추가된 부분
    EffectCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EffectCollision")); // 추가
    EffectCollision->SetupAttachment(RootComponent);                                     // 추가
    EffectCollision->SetSphereRadius(1000.0f);                                            // 추가
    EffectCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);                // 추가
    EffectCollision->SetCollisionProfileName(TEXT("OverlapAll"));                        // 추가
    EffectCollision->OnComponentBeginOverlap.AddDynamic(this, &ATFT_Button::OnEffectOverlap); // 추가


    bIsOn = false;
    bPlayerIsNearby = false;
}

// Called when the game starts or when spawned
void ATFT_Button::BeginPlay()
{
	Super::BeginPlay();
    if (EffectCollision)
    {
        EffectCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        UE_LOG(LogTemp, Log, TEXT("EffectCollision is disabled at the start."));
    }
  
}

void ATFT_Button::AButtonActor()
{
}

// Called every frame
void ATFT_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATFT_Button::ToggleButton()
{
    if (bIsCooldown) // 쿨타임 중이라면 동작하지 않음
    {
        UE_LOG(LogTemp, Log, TEXT("Button is on cooldown!"));
        return;
    }

    bIsOn = !bIsOn;
    UE_LOG(LogTemp, Log, TEXT("Button is %s"), bIsOn ? TEXT("ON") : TEXT("OFF"));

    if (bIsOn)
    {
        if (EFFECTMANAGER)
        {
            FVector EffectLocation = GetActorLocation() + EffectLocationOffset;
            FRotator EffectRotation = GetActorRotation() + EffectRotationOffset;

            EFFECTMANAGER->Play(TEXT("Energy"), 0, EffectLocation, EffectRotation);

            EnableEffectCollision(); // 콜리전 활성화
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("EffectManager is null"));
        }

        // 쿨타임 시작
        StartCooldown();
    }
    else
    {
        DisableEffectCollision(); // 콜리전 비활성화
    }
}

void ATFT_Button::StartCooldown()
{
    bIsCooldown = true; // 쿨타임 상태로 전환
    UE_LOG(LogTemp, Log, TEXT("Cooldown started for 10 seconds."));

    // 10초 후 쿨타임 종료
    FTimerHandle CooldownTimerHandle;
    GetWorldTimerManager().SetTimer(CooldownTimerHandle, this, &ATFT_Button::EndCooldown, 10.0f, false);
}

void ATFT_Button::EndCooldown()
{
    bIsCooldown = false; // 쿨타임 종료
    UE_LOG(LogTemp, Log, TEXT("Cooldown ended. Button can be activated again."));
}

void ATFT_Button::EnableEffectCollision()
{
    EffectCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    UE_LOG(LogTemp, Log, TEXT("Effect Collision Enabled"));

    // 디버그 구체 생성 (초록색: 활성화 상태)
    DrawDebugSphere(GetWorld(), EffectCollision->GetComponentLocation(), EffectCollision->GetScaledSphereRadius(),
        12, FColor::Green, false, 10.0f);

    // 타이머를 사용해 10초 후 콜리전을 비활성화
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATFT_Button::DisableEffectCollision, 10.0f, false);
}

void ATFT_Button::DisableEffectCollision()
{
    EffectCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    UE_LOG(LogTemp, Log, TEXT("Effect Collision Disabled"));

    // 디버그 구체 생성 (초록색 제거)
    DrawDebugSphere(GetWorld(), EffectCollision->GetComponentLocation(), EffectCollision->GetScaledSphereRadius(),
        12, FColor::Red, false, 2.0f); // 빨간색: 비활성화 상태
}

void ATFT_Button::OnEffectOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto Player = Cast<ATFT_Player>(OtherActor); // 충돌한 객체가 플레이어인지 확인
    if (Player)
    {
        UE_LOG(LogTemp, Log, TEXT("Effect collision with player detected!"));

        // 1. 플레이어 공격력 버프 적용
        if (Player->_statCom)
        {
            Player->_statCom->AddAttackDamage(10); // 공격력 +10
            UE_LOG(LogTemp, Log, TEXT("Player attack power increased by 10!"));
        }

        // 2. 이펙트 재생
        if (EFFECTMANAGER)
        {
            EFFECTMANAGER->Play(TEXT("ATTACKUP"), 1, Player->GetActorLocation(), FRotator::ZeroRotator);
            UE_LOG(LogTemp, Log, TEXT("Attack up effect played on player!"));
        }

        // 3. 10초 후 버프 제거
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, [Player]()
            {
                if (Player->_statCom)
                {
                    Player->_statCom->AddAttackDamage(-10); // 공격력 -10
                    UE_LOG(LogTemp, Log, TEXT("Player attack power buff ended!"));
                }
            }, 10.0f, false);

        // 디버그 구체 생성 (빨간색: 충돌 발생)
        DrawDebugSphere(GetWorld(), EffectCollision->GetComponentLocation(), EffectCollision->GetScaledSphereRadius(),
            12, FColor::Red, false, 1.0f);
    }
}

void ATFT_Button::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ATFT_Player::StaticClass()))
    {
        bPlayerIsNearby = true; 
        UE_LOG(LogTemp, Log, TEXT("Player is near the button"));
    }
}

void ATFT_Button::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(ATFT_Player::StaticClass()))
    {
        bPlayerIsNearby = false; // 플레이어가 근처에서 벗어남
        UE_LOG(LogTemp, Log, TEXT("Player left the button"));
    }
}

