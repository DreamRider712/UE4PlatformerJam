// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

AEnemyBase::AEnemyBase() {
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::OnOverlapEnd);
}

void AEnemyBase::Attack(){
}

//void AEnemyBase::TakeDamage(){
//}

void AEnemyBase::Death(){
	bCanBeDamaged = false;
	if (DeathAnimation) {
		GetSprite()->SetLooping(false);
		GetSprite()->SetFlipbook(DeathAnimation);
	}
}

void AEnemyBase::Patrol(){
}

void AEnemyBase::ChangeStatus(EEnemyStatus Status){
}

void AEnemyBase::ResetAnimation() {
	GetSprite()->SetFlipbook(IdleAnimation);
	bCanBeDamaged = true;
}

void AEnemyBase::ReceiveDamage(float value) {
	FTimerHandle TimerHandle;
	if (bCanBeDamaged) {
		if (Health - value <= 0) {
			Health -= value;
			Death();
		}
		else {
			Health -= value;
			GetSprite()->SetFlipbook(HurtAnimation);
			bCanBeDamaged = false;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::ResetAnimation, 0.4f);
		}
	}
}

void AEnemyBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	UE_LOG(LogTemp, Warning, TEXT("DAMAGING ENEMY"));
}

void AEnemyBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}
