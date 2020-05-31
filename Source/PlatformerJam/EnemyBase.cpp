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
	bIsAttacking = false;
	bChasing = false;

	CurrentStatus = EEnemyStatus::ES_Idle;
	
	Speed = 0.1f;
	MinAttackTime = 0.2f;
	MaxAttackTime = 1.0f;
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
	TargetPoint += GetActorLocation();
	
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
	if (!bIsAttacking && bCanBeDamaged) {
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
		case EEnemyStatus::ES_EndChase:
			GetSprite()->SetFlipbook(WalkAnimation);
			break;
		//case EEnemyStatus::ES_Combat:
		//	GetSprite()->SetFlipbook(AttackAnimation);
		//	break;
		case EEnemyStatus::ES_Death:
			GetSprite()->SetFlipbook(DeathAnimation);
			break;
		default:
			break;
		}
	}
}

//Function used as the AI of the enemy
void AEnemyBase::StartAI() {
	if (Speed == 0.f) {
		ChangeStatus(EEnemyStatus::ES_Idle);
	}
	/** We want to make sure to reset damage collisions and damage times as long as we are out of combat*/
	if (!bIsAttacking && CurrentStatus != EEnemyStatus::ES_Combat) {
		if (GetWorldTimerManager().IsTimerActive(attackTimer))
			GetWorldTimerManager().ClearTimer(attackTimer);
		if (GetWorldTimerManager().IsTimerActive(activateDamageTimer))
			GetWorldTimerManager().ClearTimer(activateDamageTimer);
		//Deactivate collision in case it wasn't deactivated in time
		DeactivateCollision();
	}
	if (bCanBeDamaged && !bIsAttacking) {
		switch (CurrentStatus)
		{
		case EEnemyStatus::ES_Idle:
			//We want enemy to start in wait for some seconds, then it can start patrolling
			if (!bChasing) {
				ActionWait();
			}
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
			break;
		default:
			break;
		}
	}
	//	ChangeStatus(EEnemyStatus::ES_Patrol);
}

void AEnemyBase::ActionWait() {
	ChangeStatus(EEnemyStatus::ES_Patrol);
	Speed = 0.1f;
}

void AEnemyBase::Patrol() {
	FVector CurrentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	if (ForwardVector.X < 0.f && CurrentLocation.X > PointB.X || ForwardVector.X > 0.f && CurrentLocation.X < PointB.X) {
		UE_LOG(LogTemp, Warning, TEXT("SHOULD CARRY ON"));
		AddMovementInput(ForwardVector, Speed);
	}
	else {
		ChangeStatus(EEnemyStatus::ES_Idle);
		UE_LOG(LogTemp, Warning, TEXT("SHOULD FLIP"));
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
	if (CurrentStatus == EEnemyStatus::ES_Idle) {
		ChangeStatus(EEnemyStatus::ES_Patrol);
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
	if (FVector::Dist(CurrentLocation, PointB) > 7.f) {
		AddMovementInput(ForwardVector, Speed);
	}
	else {
		ChangeStatus(EEnemyStatus::ES_Idle);
		GetWorldTimerManager().SetTimer(resetHandle, this, &AEnemyBase::ResetPosition, 3.f);
	}
}

//This function is used so the enemy goes back to its original patrolling path if loses sight of enemy
void AEnemyBase::ResetPosition() {
	bChasing = false;
	Speed = 0.1f;
	FVector ForwardVector = GetActorForwardVector();
	FVector CurrentLocation = GetActorLocation();
	if (FVector::Dist(CurrentLocation, StartPoint) > 1.f) {
		PointB = StartPoint;
		if (ForwardVector.X > 0 && CurrentLocation.X >= PointB.X + 5.f || ForwardVector.X < 0 && CurrentLocation.X < PointB.X - 5.f) {
			FlipEnemy();
		}
		AddMovementInput(ForwardVector, Speed);
	}
	else {
		PointB = TargetPoint;
		ChangeStatus(EEnemyStatus::ES_Patrol);
	}
}

void AEnemyBase::CombatStart() {
	//Gotta make sure we aren't chasing the enemy
	bChasing = false;
	if (GetWorldTimerManager().IsTimerActive(resetHandle)) {
		GetWorldTimerManager().ClearTimer(resetHandle);
	}
	if (!bIsAttacking && bCanBeDamaged) {
		bIsAttacking = true;
		GetSprite()->SetFlipbook(AttackAnimation);
		float currentAnimTime = GetSprite()->GetFlipbookLength();
		UE_LOG(LogTemp, Warning, TEXT("ATTACKING"));
		//Should deal damage mid-way through the attack animation
		GetWorldTimerManager().SetTimer(activateDamageTimer, this, &AEnemyBase::ActivateCollision, currentAnimTime * 3 / 4);
		GetWorldTimerManager().SetTimer(endAttackTimer, this, &AEnemyBase::EndAttack, currentAnimTime);
	}
}

void AEnemyBase::EndAttack() {
	bIsAttacking = false;
	//First Deactivate the collision and reset activateDamageTimer
	DeactivateCollision();
	GetWorldTimerManager().ClearTimer(activateDamageTimer);
	//Make sure we are not attacking continuously, add a random delay between attacks
	float attackRate = FMath::FRandRange(MinAttackTime, MaxAttackTime);
	if (bCloseColliderOverlap && bCanBeDamaged) {
		GetSprite()->SetFlipbook(IdleAnimation);
		UE_LOG(LogTemp, Warning, TEXT("REPEATING ATTACK AT RATE: %f"), attackRate);
		GetWorldTimerManager().ClearTimer(endAttackTimer);
		GetWorldTimerManager().SetTimer(attackTimer, this, &AEnemyBase::CombatStart, attackRate);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ENDING ATTACK"));
	}
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
	//If target is still colliding with close combat collider, we should go back to attacking
	//Recovery rate should be lower so AI doesn't feel "too easy"
	FTimerHandle RestartCombat;
	float attackRate = FMath::FRandRange(0.01f, MinAttackTime);
	if (bCloseColliderOverlap) {
		GetWorldTimerManager().SetTimer(RestartCombat, this, &AEnemyBase::CombatStart, attackRate);
	}
}

void AEnemyBase::ReceiveDamage(float value) {
	FTimerHandle TimerHandle;
	//When taking damage, make sure we don't damage target without actually playing attack animation
	DeactivateCollision();
	GetWorldTimerManager().ClearTimer(activateDamageTimer);
	if (bCanBeDamaged) {
		bCanBeDamaged = false;
		if (Health - value <= 0) {
			Health -= value;
			GetSprite()->SetFlipbook(HurtAnimation);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::Death, GetSprite()->GetFlipbookLength());
			//Death();
		}
		else {
			Health -= value;
			GetSprite()->SetFlipbook(HurtAnimation);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::ResetAnimation, GetSprite()->GetFlipbookLength());
		}
	}
}


void AEnemyBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
}

void AEnemyBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			UE_LOG(LogTemp, Warning, TEXT("SHOULD DAMAGE ENEMY"));
			MainChar->ReceiveDamage(Damage);
		}
	}
}

void AEnemyBase::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			UE_LOG(LogTemp, Warning, TEXT("SHOULD STOP DAMAGING ENEMY"));
		}
	}
}

void AEnemyBase::SensorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	// If we "detect" enemy (i.e. Player) then we'll start chasing
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			// First change status to Chase
			bChasing = true;
			if (GetWorldTimerManager().IsTimerActive(resetHandle)) {
				GetWorldTimerManager().ClearTimer(resetHandle);
			}
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
			PointB = TargetPoint;
		}
	}
}

void AEnemyBase::CloseOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			ChangeStatus(EEnemyStatus::ES_Combat);
			bCloseColliderOverlap = true;
			//Are we fasing same direction as enemy?
			if (GetActorLocation().X <= MainChar->GetActorLocation().X) {
				SetActorRotation(FRotator(0.f, 0.f, 0.f));
			}
			else {
				SetActorRotation(FRotator(0.f, 180.f, 0.f));
			}
			CombatStart();
		}
	}
}

void AEnemyBase::CloseOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			UE_LOG(LogTemp, Warning, TEXT("SHOULD STOP COMBAT COROUTINE"));
			bCloseColliderOverlap = false;
			ChangeStatus(EEnemyStatus::ES_EndChase);
			Speed = 0.1f;
		}
	}
}

void AEnemyBase::ActivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyBase::DeactivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}