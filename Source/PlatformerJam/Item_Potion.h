// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Item_Potion.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMERJAM_API AItem_Potion : public AItemBase
{
	GENERATED_BODY()

public:
	AItem_Potion();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float value;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
