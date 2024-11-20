// Fill out your copyright notice in the Description page of Project Settings.


#include "TFT_AI/TFT_NPC.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "TFT_Player.h"

// Sets default values
ATFT_NPC::ATFT_NPC()
{
   
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    RootComponent = InteractionBox;
    InteractionBox->SetCollisionProfileName(TEXT("Trigger")); 
    InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore); 
    InteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

   
    NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));
    NPCMesh->SetupAttachment(RootComponent);

}

void ATFT_NPC::OnDialogueClosed()
{
    //if (GateActor) // 게이트가 연결되어 있다면
    //{
    //    UFunction* OpenGateFunction = GateActor->FindFunction(FName("OpenGate"));
    //    if (OpenGateFunction)
    //    {
    //        GateActor->ProcessEvent(OpenGateFunction, nullptr); // 게이트 열기 호출
    //    }
    //}
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
    if (Player && DialogueWidgetClass)
    {
       
        UUserWidget* DialogueWidget = CreateWidget<UUserWidget>(GetWorld(), DialogueWidgetClass);
        if (DialogueWidget)
        {
            DialogueWidget->AddToViewport();
        }
    }
}

// Called every frame
void ATFT_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

