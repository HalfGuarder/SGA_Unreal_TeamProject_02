// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AI/TFT_NPC.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "TFT_Player.h"

// Sets default values
ATFT_NPC::ATFT_NPC()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/DialogueWidget.DialogueWidget_C'"));
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

void ATFT_NPC::OnDialogueClosed()
{
    if (DialogueWidgetInstance)
    {
        DialogueWidgetInstance->RemoveFromViewport();
        DialogueWidgetInstance = nullptr;
    }

    bIsDialogueActive = false;
    UE_LOG(LogTemp, Log, TEXT("NPC1: Dialogue UI closed."));
}

void ATFT_NPC::HideNPC()
{
    // NPC 메시와 콜리전을 숨기기
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    UE_LOG(LogTemp, Log, TEXT("NPC hidden and collision disabled."));
}

void ATFT_NPC::ShowNPC()
{
    // NPC 메시와 콜리전을 다시 활성화
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    UE_LOG(LogTemp, Log, TEXT("NPC shown and collision enabled."));
}

// Called when the game starts or when spawned
void ATFT_NPC::BeginPlay()
{
	Super::BeginPlay();
	
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ATFT_NPC::OnPlayerOverlap);
}

void ATFT_NPC::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATFT_Player* Player = Cast<ATFT_Player>(OtherActor);
    if (Player)
    {
        UE_LOG(LogTemp, Log, TEXT("NPC notifying player to open dialogue UI."));
        Player->StartDialogueUI(); 
    }
}

// Called every frame
void ATFT_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

