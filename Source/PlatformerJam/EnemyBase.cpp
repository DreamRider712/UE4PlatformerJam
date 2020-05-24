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



AEnemyBase::AEnemyBase() {
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::OnOverlapEnd);

	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SensorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SensorSphere"));
	SensorSphere->SetupAttachment(GetRootComponent());
	
	CloseRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CloseRangeSphere"));
	CloseRangeSphere->SetupAttachment(GetRootComponent());

	bIsAlive = true;
}

void AEnemyBase::BeginPlay() {
	Super::BeginPlay();
	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CombatOverlapBegin);
	CombatBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::CombatOverlapEnd);

	SensorSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::SensorOverlapBegin);
	SensorSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::SensorOverlapEnd);

	CloseRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CloseOverlapBegin);
	CloseRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::CloseOverlapEnd);

	InitialLocation = GetActorLocation();
	MoveToLocation = InitialLocation - FVector(120.f, 0.f, 0.f);
}

void AEnemyBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if(bIsAlive)
		StartAI();
}

//Function used as the AI of the enemy
void AEnemyBase::StartAI() {
	Patrol();
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
		bIsAlive = false;
	}
}

void AEnemyBase::DestroyMe() {
	Destroy();
}

void AEnemyBase::Patrol() {
	FVector ForwardVector = GetActorForwardVector();
	FVector CurrentLocation = GetActorLocation();
	if ((ForwardVector.X < 0) && CurrentLocation.X >= MoveToLocation.X && bCanBeDamaged || (ForwardVector.X > 0) && (CurrentLocation.X <= MoveToLocation.X && bCanBeDamaged)){
		GetSprite()->SetFlipbook(WalkAnimation);
		AddMovementInput(ForwardVector, 0.1f);
	}
	else {
		FVector Temp = MoveToLocation;
		MoveToLocation = InitialLocation;
		InitialLocation = Temp;
		if (ForwardVector.X < 0)
			SetActorRotation(FRotator(0.f, 0.f, 0.f));
		if (ForwardVector.X > 0)
			SetActorRotation(FRotator(0.f, 180.f, 0.f));
	}
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
		if (MainChar) {
			UE_LOG(LogTemp, Warning, TEXT("I'M ABOUT TO REKT PLAYER"));
		}
	}
}

void AEnemyBase::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::SensorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor) {
		UE_LOG(LogTemp, Warning, TEXT("DETECTED OTHERACTOR"));
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			UE_LOG(LogTemp, Warning, TEXT("DETECTED PLAYER"));
		}
	}
}

void AEnemyBase::SensorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::CloseOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			UE_LOG(LogTemp, Warning, TEXT("I'M ABOUT TO REKT PLAYER"));
		}
	}
}

void AEnemyBase::CloseOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}

void AEnemyBase::ActivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyBase::DeactivateCollision() {
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}