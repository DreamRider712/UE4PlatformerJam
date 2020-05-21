// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PaperFlipbookComponent.h"

AEnemyBase::AEnemyBase() {

}

void AEnemyBase::Attack(){
}

//void AEnemyBase::TakeDamage(){
//}

void AEnemyBase::Death(){
	if(DeathAnimation)
		GetSprite()->SetFlipbook(DeathAnimation);
}

void AEnemyBase::Patrol(){
}

void AEnemyBase::ChangeStatus(EEnemyStatus Status){
}

void AEnemyBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
}

void AEnemyBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}
