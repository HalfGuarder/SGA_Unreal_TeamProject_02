// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_BossMonster_CG.h"

#include "Components/SkeletalMeshComponent.h"

ATFT_BossMonster_CG::ATFT_BossMonster_CG()
{
	_invenCom = CreateDefaultSubobject<UTFT_InvenComponent>(TEXT("Inven_Com"));

    // Body mesh component 초기화 및 설정
    BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMeshComponent"));
    BodyMeshComponent->SetupAttachment(RootComponent);

    //// Helmet mesh component 초기화 및 설정
    HelmetMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMeshComponent"));
    HelmetMeshComponent->SetupAttachment(RootComponent);

    // Body와 Helmet의 메시 경로 설정
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshAsset(TEXT("/Game/Custom_CG/charaecter/Meshes/SK_CombatSuit_Base_No_Head.SK_CombatSuit_Base_No_Head"));
    if (BodyMeshAsset.Succeeded())
    {
        BodyMeshComponent->SetSkeletalMesh(BodyMeshAsset.Object);

        BodyMeshComponent->SetRelativeLocationAndRotation
        (FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> HelmetMeshAsset(TEXT("/Game/Custom_CG/charaecter/Meshes/SK_ArmourWCoat.SK_ArmourWCoat"));
    if (HelmetMeshAsset.Succeeded())
    {
        HelmetMeshComponent->SetSkeletalMesh(HelmetMeshAsset.Object);

        HelmetMeshComponent->SetRelativeLocationAndRotation
        (FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
    }

    //// Helmet mesh가 Body mesh 애니메이션을 따라가도록 설정
    HelmetMeshComponent->SetMasterPoseComponent(BodyMeshComponent);

}

void ATFT_BossMonster_CG::BeginPlay()
{
    Super::BeginPlay();

    // BeginPlay에서도 다시 Master Pose 설정 가능
    HelmetMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
}



