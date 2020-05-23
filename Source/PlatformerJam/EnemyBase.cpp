// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "MainChar.h"
#include "TimerManager.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"



AEnemyBase::AEnemyBase() {
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::OnOverlapEnd);

	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SensorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SensorSphere"));
	CloseRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CloseRangeSphere"));

}

void AEnemyBase::BeginPlay() {
	Super::BeginPlay();
	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CombatOverlapBegin);
	CombatBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::CombatOverlapEnd);
}

void AEnemyBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	StartAI();
}

//Function used as the AI of the enemy
void AEnemyBase::StartAI() {

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
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::DestroyMe, 2.f);
	}
}

void AEnemyBase::DestroyMe() {
	Destroy();
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
			GetSprite()->SetFlipbook(HurtAnimation);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::Death, 0.4f);
			//Death();
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

void AEnemyBase::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (OtherActor) {
			//TODO: Add logic to damage player
		}
	}
}

void AEnemyBase::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::ActivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyBase::DeactivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}