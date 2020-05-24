// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "EnemyBase.h"
#include "Classes/Kismet/GameplayStatics.h"

AMainChar::AMainChar(){
	// Make sure to only use Yaw from controller and ignore rest of rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//Initialize camera and boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bAbsoluteRotation = true;
	SpringArm->RelativeRotation = FRotator(0.f, -90.f, 0.f);
	SpringArm->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	SpringArm->bAbsoluteRotation = true;
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));

	LowAttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LowAttackBox"));
	LowAttackBox->SetupAttachment(GetSprite());

	LowAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LowAttackBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LowAttackBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LowAttackBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	HighAttackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HighAttackBox"));
	HighAttackBox->SetupAttachment(GetSprite());

	HighAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HighAttackBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HighAttackBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HighAttackBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	bIsAttacking = false;
	LastAttackAnimation = AttackAnimation_2;
	bIsAlive = true;

	Health = 200.f;
	MaxHealth = 200.f;
}

void AMainChar::BeginPlay() {
	Super::BeginPlay();

	LowAttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMainChar::CombatOverlapBegin);
	LowAttackBox->OnComponentEndOverlap.AddDynamic(this, &AMainChar::CombatOverlapEnd);
	HighAttackBox->OnComponentBeginOverlap.AddDynamic(this, &AMainChar::CombatOverlapBegin);
	HighAttackBox->OnComponentEndOverlap.AddDynamic(this, &AMainChar::CombatOverlapEnd);
}

void AMainChar::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (bIsAlive) {
		UpdateCharacter();
	}
	
}

void AMainChar::Jump() {
	if (bIsAlive) {
		Super::Jump();
	}
}

void AMainChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainChar::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainChar::Attack);
	//PlayerInputComponent->BindAction("Attack", IE_Released, this, &AMainChar::Attack);
}

void AMainChar::MoveRight(float value) {
	if (!bIsAttacking && bIsAlive) {
		AddMovementInput(FVector(1.f, 0.f, 0.f), value);
	}
}

void AMainChar::UpdateCharacter() {
	//1. Update Animation
	UpdateAnimation();
	//2. Setup Rotation of the controller based on direction
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;

	//3. Set rotation so that character faces direction of travel
	if (Controller != nullptr) {
		if (TravelDirection < 0.f) {
			Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));
		}
		else if (TravelDirection > 0.f) {
			Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
		}
	}

}

void AMainChar::UpdateAnimation() {
	bool bIsFalling = GetMovementComponent()->IsFalling();
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	if (!bIsFalling) {
		if (!bIsAttacking) {
			UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.f) ? RunAnimation : IdleAnimation;
			if (GetSprite()->GetFlipbook() != DesiredAnimation) {
				GetSprite()->SetFlipbook(DesiredAnimation);
			}
		}
		else {
			GetSprite()->SetFlipbook(LastAttackAnimation);
		}
	}
	else {
		if (GetSprite()->GetFlipbook() != JumpAnimation)
			GetSprite()->SetFlipbook(JumpAnimation);
	}
}

void AMainChar::Attack() {
	bool bIsFalling = GetMovementComponent()->IsFalling();
	if (!bIsFalling) {
		if (!bIsAttacking) {
			LastAttackAnimation = LastAttackAnimation == AttackAnimation_1 ? AttackAnimation_2 : AttackAnimation_1;
			bIsAttacking = true; 
			if (LastAttackAnimation == AttackAnimation_1)
				ActivateCollision(LowAttackBox);
			else
				ActivateCollision(HighAttackBox);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AMainChar::EndAttack, .36f);
		}																																																																																																																			
	}
}

void AMainChar::EndAttack() {
	bIsAttacking = false;
	if (LastAttackAnimation == AttackAnimation_1)
		DeactivateCollision(LowAttackBox);
	else
		DeactivateCollision(HighAttackBox);
}

void AMainChar::ReceiveDamage(float value) {
	if (Health - value <= 0) {
		Health -= value;
		Death();
	}else {
		Health -= value;
	}
}

void AMainChar::Death() {
	if (DeathAnimation) {
		UE_LOG(LogTemp, Warning, TEXT("I DIED"));
		GetSprite()->SetFlipbook(DeathAnimation);
		GetSprite()->SetLooping(false);

		bCanBeDamaged = false;
		bIsAlive = false;
	}
}

void AMainChar::ActivateCollision(UBoxComponent* Comp) {
	Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainChar::DeactivateCollision(UBoxComponent* Comp) {
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainChar::CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if (OtherActor) {
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
		if (Enemy) {
			Enemy->ReceiveDamage(10.f);
		}
	}
}

void AMainChar::CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
}
