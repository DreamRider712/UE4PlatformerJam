// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharController.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMERJAM_API AMainCharController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainCharController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "widgets")
	UUserWidget* HUDOverlay;

protected:
	virtual void BeginPlay() override;
};
