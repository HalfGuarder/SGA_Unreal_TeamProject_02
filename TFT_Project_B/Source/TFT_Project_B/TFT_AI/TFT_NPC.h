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
protected:
	
    // Components
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* InteractionBox;

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
};
