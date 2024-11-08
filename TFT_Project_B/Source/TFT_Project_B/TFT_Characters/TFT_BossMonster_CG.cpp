// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_Characters/TFT_BossMonster_CG.h"

#include "Components/SkeletalMeshComponent.h"

ATFT_BossMonster_CG::ATFT_BossMonster_CG()
{
	_invenCom = CreateDefaultSubobject<UTFT_InvenComponent>(TEXT("Inven_Com"));

    // Body mesh component �ʱ�ȭ �� ����
    BodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMeshComponent"));
    BodyMeshComponent->SetupAttachment(RootComponent);

    //// Helmet mesh component �ʱ�ȭ �� ����
    HelmetMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMeshComponent"));
    HelmetMeshComponent->SetupAttachment(RootComponent);

    // Body�� Helmet�� �޽� ��� ����
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

    //// Helmet mesh�� Body mesh �ִϸ��̼��� ���󰡵��� ����
    HelmetMeshComponent->SetMasterPoseComponent(BodyMeshComponent);

}

void ATFT_BossMonster_CG::BeginPlay()
{
    Super::BeginPlay();

    // BeginPlay������ �ٽ� Master Pose ���� ����
    HelmetMeshComponent->SetMasterPoseComponent(BodyMeshComponent);
}



