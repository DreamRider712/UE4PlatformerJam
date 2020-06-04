// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RequireKeyVolume.generated.h"

UCLASS()
class PLATFORMERJAM_API ARequireKeyVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARequireKeyVolume();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyQuery")
	class UBoxComponent* CollisionComponent;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "widgets")
	TSubclassOf<class UUserWidget> HUDMessageAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "widgets")
	UUserWidget* HUDMessage;
	
	FTimerHandle ResetMsgTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetMessageScreen();

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
