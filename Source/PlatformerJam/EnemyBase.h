// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyStatus : uint8 {
	ES_Idle		UMETA(DisplayName = "Idle"),
	ES_Patrol	UMETA(DisplayName = "Patrolling"),
	ES_Combat	UMETA(DisplayName = "Combat"),
	ES_Death	UMETA(DsiplayName = "Death"),
	ES_MAX		UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class PLATFORMERJAM_API AEnemyBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	//Constructor
	AEnemyBase();

	//For Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* WalkAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* HurtAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DeathAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* CombatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* SensorSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* CloseRangeSphere;



	//Functions for gameplay (Attack, damage, death, patrol)
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void Attack();
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void DealDamage();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void EndAttack();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void Patrol();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void Death();

	FTimerHandle resetTimerHandle;
	FTimerHandle damageTimerHandle;
	FTimerHandle attackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	void ActivateCollision();
	void DeactivateCollision();

	void StartAI();
	
	//SETTERS-GETTERS
	FORCEINLINE void SetHealth(float value) { Health = value; }
	FORCEINLINE void SetMaxHealth(float value) { MaxHealth = value; }
	FORCEINLINE void SetDamage(float value) { Damage = value; }

	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetDamage() { return Damage; }

	FORCEINLINE void ChangeStatus(EEnemyStatus Status) { CurrentStatus = Status; }

	//Basic Variables; Set as private, should be used only through child class through setters-getters
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float Damage;

	float Speed;

	void ResetAnimation();

	FVector InitialLocation;
	FVector MoveToLocation;

	bool bIsAlive;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	bool bOverlappingCombatSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	bool bIsAttacking;


	//Items related to enemy status enum
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	EEnemyStatus CurrentStatus;

	void ReceiveDamage(float value);

	virtual void Tick(float DeltaSeconds) override;

	void DestroyMe();

	void ChaseEnemy(FVector TargetPosition);

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void SensorOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SensorOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void CloseOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CloseOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
};
