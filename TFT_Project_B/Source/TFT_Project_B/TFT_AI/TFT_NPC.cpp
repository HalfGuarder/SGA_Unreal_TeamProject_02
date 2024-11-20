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
    if (GateActor) // 게이트가 연결되어 있다면
    {
        UFunction* OpenGateFunction = GateActor->FindFunction(FName("OpenGate"));
        if (OpenGateFunction)
        {
            GateActor->ProcessEvent(OpenGateFunction, nullptr); // 게이트 열기 호출
        }
    }
}

// Called when the game starts or when spawned
void ATFT_NPC::BeginPlay()
{
	Super::BeginPlay();
	
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ATFT_NPC::OnPlayerOverlap);
}

void ATFT_NPC::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("OnPlayerOverlap triggered")); // 이벤트 호출 여부 확인

    ATFT_Player* Player = Cast<ATFT_Player>(OtherActor);
    if (Player && DialogueWidgetClass)
    {
        UUserWidget* DialogueWidget = CreateWidget<UUserWidget>(GetWorld(), DialogueWidgetClass);
        if (DialogueWidget)
        {
            DialogueWidget->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("DialogueWidget added to viewport."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create DialogueWidget."));
        }
    }
    else
    {
        if (!DialogueWidgetClass)
        {
            UE_LOG(LogTemp, Error, TEXT("DialogueWidgetClass is NULL."));
        }
        if (!Player)
        {
            UE_LOG(LogTemp, Warning, TEXT("OtherActor is not a player."));
        }
    }
}

// Called every frame
void ATFT_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

