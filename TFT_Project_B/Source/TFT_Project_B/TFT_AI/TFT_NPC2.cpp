// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AI/TFT_NPC2.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "TFT_Player.h"

// Sets default values
ATFT_NPC2::ATFT_NPC2()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/DialogueWidget2.DialogueWidget2_C'"));
    if (WidgetClass.Succeeded())
    {
        DialogueWidgetClass = WidgetClass.Class;
        UE_LOG(LogTemp, Log, TEXT("DialogueWidget class loaded successfully."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load DialogueWidget class. Check the path."));
    }

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    RootComponent = InteractionBox;
    InteractionBox->SetCollisionProfileName(TEXT("Trigger"));
    InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // 'TFT_Ally' 채널과만 겹침


    NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));
    NPCMesh->SetupAttachment(RootComponent);
	

}

void ATFT_NPC2::OnDialogueClosed()
{
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->RemoveFromViewport();
        DialogueWidgetInstance = nullptr;
    }

    bIsDialogueActive = false;
    UE_LOG(LogTemp, Log, TEXT("NPC1: Dialogue UI closed."));
}

void ATFT_NPC2::HideNPC()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    UE_LOG(LogTemp, Log, TEXT("NPC hidden and collision disabled."));
}

void ATFT_NPC2::ShowNPC()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    UE_LOG(LogTemp, Log, TEXT("NPC shown and collision enabled."));
}


void ATFT_NPC2::BeginPlay()
{
	Super::BeginPlay();

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ATFT_NPC2::OnPlayerOverlap);

    HideNPC();
}


void ATFT_NPC2::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATFT_Player* Player = Cast<ATFT_Player>(OtherActor);
    if (Player)
    {
        UE_LOG(LogTemp, Log, TEXT("NPC2 notifying player to open dialogue UI2."));
        Player->StartDialogueUI2(); // 변경된 메서드 호출
    }
}

void ATFT_NPC2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

