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
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation_2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* HurtAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* LowAttackBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* HighAttackBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	float Stamina;
	float MaxStamina;
	float Damage;

	//Adding a slight delay to weapon so combat isn't linear
	float WeaponDelay;

	bool bIsAlive;
	bool bCanBeDamaged;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	bool bCanAttack;

	FORCEINLINE void SetHealth(float value) { Health = value; }
	FORCEINLINE void SetMaxHealth(float value) { MaxHealth = value; }
	FORCEINLINE void SetKey(int32 value) { KeyCount = value; }

	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE int32 GetKeyCount() { return KeyCount; }

	FTimerHandle timerHandle;
	FTimerHandle attackDelayHandle;

	int32 KeyCount;

	class AMainCharController* controllerRef;

	//To keep track of which animation was used
	class UPaperFlipbook* LastAttackAnimation;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	void MoveRight(float value);
	void UpdateAnimation();
	void UpdateCharacter();
	void ResetDamage();
	void ResetWeaponDelay();

	//Functions used for combat
	void Attack();
	void EndAttack();
	void ReceiveDamage(float value);
	void Death();

	void ActivateCollision(UBoxComponent* Comp);
	void DeactivateCollision(UBoxComponent* Comp);

	virtual void Jump() override;

	UFUNCTION()
	void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

};
