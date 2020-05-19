// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

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
}

void AMainChar::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	UpdateCharacter();
}

void AMainChar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}

void AMainChar::MoveRight(float value) {
	AddMovementInput(FVector(1.f, 0.f, 0.f), value);
	
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
	bool bIsGrounded = GetMovementComponent()->IsFalling();
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	if (!bIsGrounded) {
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.f) ? RunAnimation : IdleAnimation;
		if (GetSprite()->GetFlipbook() != DesiredAnimation) {
			GetSprite()->SetFlipbook(DesiredAnimation);
		}
	}
	else {
		if (GetSprite()->GetFlipbook() != JumpAnimation)
			GetSprite()->SetFlipbook(JumpAnimation);
	}
}