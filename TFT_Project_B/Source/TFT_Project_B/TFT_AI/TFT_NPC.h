// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TFT_NPC.generated.h"


class UUserWidget;

UCLASS()
class TFT_PROJECT_B_API ATFT_NPC : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ATFT_NPC();

    UFUNCTION()
    void OnDialogueClosed();

    // 추가된 함수
    UFUNCTION()
    void HideNPC(); // NPC 숨기기 및 콜리전 비활성화

    UFUNCTION()
    void ShowNPC(); // NPC 다시 표시 및 콜리전 활성화
protected:
	


    UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent* NPCMesh;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;



    // Overlap Event
    UFUNCTION()
    void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	
	virtual void Tick(float DeltaTime) override;
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DialogueWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate")
    AActor* GateActor;

    // Components
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* InteractionBox;

    UPROPERTY()
    UUserWidget* DialogueWidgetInstance;

    // 대화 활성화 상태
    bool bIsDialogueActive;
};
