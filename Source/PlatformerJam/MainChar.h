// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MainChar.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMERJAM_API AMainChar : public APaperCharacter
{
	GENERATED_BODY()

public:
	//Constructor
	AMainChar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipBook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComponent;

};
