// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Goblin.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "ItemBase.h"

AEnemy_Goblin::AEnemy_Goblin() {

}

void AEnemy_Goblin::BeginPlay() {
	Super::BeginPlay();

	ChangeStatus(CurrentStatus);
}

void AEnemy_Goblin::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

void AEnemy_Goblin::CheckStatus(EEnemyStatus Status) {

}

void AEnemy_Goblin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AEnemy_Goblin::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}