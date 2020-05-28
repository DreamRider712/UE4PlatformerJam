// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "MainChar.h"
#include "TimerManager.h"
#include "ConstructorHelpers.h"
#include "GameFramework/Controller.h"
#include "Classes/AIController.h"



AEnemyBase::AEnemyBase() {
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::OnOverlapEnd);

	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CombatBox->SetupAttachment(GetRootComponent());

	SensorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SensorSphere"));
	SensorSphere->SetupAttachment(GetRootComponent());
	
	CloseRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CloseRangeSphere"));
	CloseRangeSphere->SetupAttachment(GetRootComponent());

	bIsAlive = true;
	bOverlappingCombatSphere = false;
	bIsAttacking = false;

	CurrentStatus = EEnemyStatus::ES_Idle;

	StartPoint = FVector(0.f);
	TargetPoint = FVector(0.f);

	Speed = 0.1f;
}

void AEnemyBase::BeginPlay() {
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());
	
	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CombatOverlapBegin);
	CombatBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::CombatOverlapEnd);
	
	SensorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::SensorOverlapBegin);
	SensorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::SensorOverlapEnd);

	CloseRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CloseOverlapBegin);
	CloseRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::CloseOverlapEnd);

	StartPoint = GetActorLocation();
	TargetPoint += StartPoint;
	PointA = StartPoint;
	PointB = TargetPoint;
}

void AEnemyBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (bIsAlive) {
		StartAI();
		UpdateAnimation();
	}
}

void AEnemyBase::UpdateAnimation() {
	switch (CurrentStatus)
	{
	case EEnemyStatus::ES_Idle:
		GetSprite()->SetFlipbook(IdleAnimation);
		break;
	case EEnemyStatus::ES_Patrol:
		GetSprite()->SetFlipbook(WalkAnimation);
		break;
	case EEnemyStatus::ES_Chase:
		GetSprite()->SetFlipbook(WalkAnimation);
		break;
	case EEnemyStatus::ES_Combat:
		break;
	case EEnemyStatus::ES_Death:
		break;
	case EEnemyStatus::ES_MAX:
		break;
	default:
		break;
	}

}

//Function used as the AI of the enemy
void AEnemyBase::StartAI() {
	if (Speed == 0.f) {
		ChangeStatus(EEnemyStatus::ES_Idle);
	}
	if (bCanBeDamaged) {
		switch (CurrentStatus)
		{
		case EEnemyStatus::ES_Idle:
			//We want enemy to start in wait for some seconds, then it can start patrolling
			ActionWait();
			break;
		case EEnemyStatus::ES_Patrol:
			Patrol();
			break;
		case EEnemyStatus::ES_Chase:
			ChaseEnemy();
			break;
		case EEnemyStatus::ES_EndChase:
			ResetPosition();
			break;
		case EEnemyStatus::ES_Combat:
			Attack();
			break;
		default:
			break;
		}
	}

	//if (!bOverlappingCombatSphere && !bIsAttacking)
	//	ChangeStatus(EEnemyStatus::ES_Patrol);
}

void AEnemyBase::ActionWait() {
	ChangeStatus(EEnemyStatus::ES_Patrol);
	Speed = 0.1f;
}

void AEnemyBase::Patrol() {
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	//FTimerHandle WaitHandle;
	if (ForwardVector.X < 0.f && CurrentLocation.X > PointB.X || ForwardVector.X > 0.f && CurrentLocation.X < PointB.X) {
		AddMovementInput(ForwardVector, Speed);
	}
	else {
		FVector Temp = PointA;
		PointA = PointB;
		PointB = Temp;
		Speed = 0.f;
		FlipEnemy();
		//GetWorldTimerManager().SetTimer(WaitHandle, this, &AEnemyBase::FlipEnemy, 2.f);
	}
}

void AEnemyBase::FlipEnemy() {
	FVector ForwardVector = GetActorForwardVector();
	if (ForwardVector.X < 0.f) {
		SetActorRotation(FRotator(0.f, 0.f, 0.f));
	}
	if (ForwardVector.X > 0.f) {
		SetActorRotation(FRotator(0.f, 180.f, 0.f));
	}
}

void AEnemyBase::ChaseEnemy() {
	Speed = 0.3f;
	FVector ForwardVector = GetActorForwardVector();
	FVector CurrentLocation = GetActorLocation();

	//Are we fasing same direction as enemy?
	if (CurrentLocation.X <= PointB.X) {
		SetActorRotation(FRotator(0.f, 0.f, 0.f));
	}
	else {
		SetActorRotation(FRotator(0.f, 180.f, 0.f));
	}
	UE_LOG(LogTemp, Warning, TEXT("DISTANCE CHECK: %f"), (FVector::Dist(CurrentLocation, PointB)));
	if (FVector::Dist(CurrentLocation, PointB) > 7.f) {
		AddMovementInput(ForwardVector, Speed);
	}
	else {
		Speed = 0.1f;
		ChangeStatus(EEnemyStatus::ES_EndChase);
		ResetPosition();
	}
}

//This function is used so the enemy goes back to its original patrolling path if loses sight of enemy
void AEnemyBase::ResetPosition() {
	FVector ForwardVector = GetActorForwardVector();
	FVector CurrentLocation = GetActorLocation();
	if (FVector::Dist(CurrentLocation, StartPoint) > 1.f) {
		PointB = StartPoint;
		if (ForwardVector.X > 0 && CurrentLocation.X > PointB.X || ForwardVector.X < 0 && CurrentLocation.X < PointB.X) {
			FlipEnemy();
		}
		AddMovementInput(ForwardVector, Speed);
	}
	else {
		PointB = TargetPoint;
		ChangeStatus(EEnemyStatus::ES_Patrol);
	}
}

void AEnemyBase::Attack(){
}

void AEnemyBase::DealDamage() {
}

void AEnemyBase::EndAttack() {
}

void AEnemyBase::Death(){
	bCanBeDamaged = false;
	if (DeathAnimation) {
		GetSprite()->SetLooping(false);
		GetSprite()->SetFlipbook(DeathAnimation);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::DestroyMe, 1.f);
		bIsAlive = false;
	}
}

void AEnemyBase::DestroyMe() {
	Destroy();
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
}

void AEnemyBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
}

void AEnemyBase::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::SensorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	// If we "detect" enemy (i.e. Player) then we'll start chasing
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			// First change status to Chase
			ChangeStatus(EEnemyStatus::ES_Chase);
			//Keep track of enemy position
			PointB = MainChar->GetActorLocation();
			//ChaseEnemy();
		}
	}
}

void AEnemyBase::SensorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			Speed = 0.1f;
			ChangeStatus(EEnemyStatus::ES_EndChase);
		}
	}
}

void AEnemyBase::CloseOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
}

void AEnemyBase::CloseOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::ActivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyBase::DeactivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}