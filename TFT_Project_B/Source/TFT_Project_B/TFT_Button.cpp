// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Button.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TFT_Characters/TFT_Player.h"
#include "TFT_Managers/TFT_EffectManager.h"
#include "TFT_GameInstance.h"

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

    // 초기 상태 설정
    bIsOn = false;
}

// Called when the game starts or when spawned
void ATFT_Button::BeginPlay()
{
	Super::BeginPlay();
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
    bIsOn = !bIsOn;
    UE_LOG(LogTemp, Log, TEXT("Button is %s"), bIsOn ? TEXT("ON") : TEXT("OFF"));

    if (bIsOn)
    {
        if (EFFECTMANAGER)
        {
            FVector EffectLocation = GetActorLocation() + EffectLocationOffset;
            FRotator EffectRotation = GetActorRotation() + EffectRotationOffset;

            // EFFECTMANAGER 매크로를 통해 이펙트 호출
            EFFECTMANAGER->Play(TEXT("Energy"), 0, EffectLocation, EffectRotation);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("EffectManager is null"));
        }
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

