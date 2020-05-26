// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Goblin.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"

AEnemy_Goblin::AEnemy_Goblin() {
	CurrentStatus = EEnemyStatus::ES_Idle;

}

void AEnemy_Goblin::BeginPlay() {
	Super::BeginPlay();

	ChangeStatus(CurrentStatus);
	UE_LOG(LogTemp, Warning, TEXT("INIAIL Health: %f"), Health);
}

void AEnemy_Goblin::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	//ChangeStatus(CurrentStatus);
}

void AEnemy_Goblin::CheckStatus(EEnemyStatus Status) {
	/*switch (Status)
	{
	case EEnemyStatus::ES_Idle:
		GetSprite()->SetFlipbook(IdleAnimation);
		break;
	case EEnemyStatus::ES_Patrol:
		GetSprite()->SetFlipbook(WalkAnimation);
		break;
	case EEnemyStatus::ES_Combat:
		GetSprite()->SetFlipbook(AttackAnimation);
		break;
	case EEnemyStatus::ES_Death:
		GetSprite()->SetFlipbook(DeathAnimation);
		break;
	default:
		break;
	}*/
}

void AEnemy_Goblin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AEnemy_Goblin::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}