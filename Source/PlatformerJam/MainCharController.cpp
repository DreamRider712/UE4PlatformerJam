// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharController.h"
#include "Blueprint/UserWidget.h"

AMainCharController::AMainCharController() {
}

void AMainCharController::BeginPlay() {
	Super::BeginPlay();
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	if (HUDOverlay) {
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainCharController::EndGameScreen() {
	if (HUDOverlay) {
		HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
		HUDOverlay->RemoveFromParent();
	}
	if (EndScreenOverlayAsset) {
		EndScreenOverlay = CreateWidget<UUserWidget>(this, EndScreenOverlayAsset);
	}
	if (EndScreenOverlay) {

		EndScreenOverlay->AddToViewport();
		EndScreenOverlay->SetVisibility(ESlateVisibility::Visible);
		bShowMouseCursor = true;
	}
}