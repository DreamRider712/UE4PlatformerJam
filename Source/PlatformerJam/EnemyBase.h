// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "ItemBase.h"
#include "EnemyBase.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FLootTable{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Loot")
	TSubclassOf<AItemBase> itemLoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Loot")
	float dropRate;
};

UENUM(BlueprintType)
enum class EEnemyStatus : uint8 {
	ES_Idle		UMETA(DisplayName = "Idle"),
	ES_Patrol	UMETA(DisplayName = "Patrolling"),
	ES_Chase    UMETA(DisplayName = "Chase"),
	ES_EndChase UMETA(DisplayName = "EndChase"),
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

	//For collisions
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* CombatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* SensorSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* CloseRangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	//Handy function to have a better order to update animations
	void UpdateAnimation();

	//Functions for gameplay (Attack, damage, death, patrol)
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void CombatStart();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void EndAttack();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void Patrol();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void ActionWait();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void Death();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual void ResetPosition();

	void ChaseEnemy();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	bool bChasing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	bool bCloseColliderOverlap;

	FTimerHandle resetHandle;
	FTimerHandle endAttackTimer;
	FTimerHandle attackTimer;
	FTimerHandle activateDamageTimer;
	FTimerHandle patrollingHandle;

	//Points for Patrolling, design-wise its better to modify them in the editor
	//Storing the values of the first two points
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector StartPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector TargetPoint;

	//Actual patrolling points

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	FVector PointA;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	FVector PointB;

	void FlipEnemy();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float Damage;

	float Speed;

	float MinAttackTime;
	float MaxAttackTime;

	void ResetAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mechanics")
	bool bIsAlive;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
	TArray<FLootTable> lootTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
	float lootRng;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mechanics")
	bool bIsAttacking;

	//Items related to enemy status enum
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gameplay")
	EEnemyStatus CurrentStatus;

	void ReceiveDamage(float value);

	void DestroyMe();

	virtual void Tick(float DeltaSeconds) override;

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
