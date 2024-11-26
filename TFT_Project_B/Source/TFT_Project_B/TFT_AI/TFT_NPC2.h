// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TFT_NPC2.generated.h"

UCLASS()
class TFT_PROJECT_B_API ATFT_NPC2 : public AActor
{
	GENERATED_BODY()
	
public:	

	ATFT_NPC2();

	UFUNCTION()
	void OnDialogueClosed();

	
	UFUNCTION()
	void HideNPC();

	UFUNCTION()
	void ShowNPC();
protected:



	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* NPCMesh;


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

	
	bool bIsDialogueActive;
};
