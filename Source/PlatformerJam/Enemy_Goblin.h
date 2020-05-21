// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Enemy_Goblin.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMERJAM_API AEnemy_Goblin : public AEnemyBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	AEnemy_Goblin();

	virtual void Tick(float DeltaTime) override;
	void CheckStatus(EEnemyStatus Status);

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};

