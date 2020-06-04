// Fill out your copyright notice in the Description page of Project Settings.


#include "RequireKeyVolume.h"
#include "Components/BoxComponent.h"
#include "MainChar.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "MainCharController.h"

// Sets default values
ARequireKeyVolume::ARequireKeyVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	CollisionComponent->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	CollisionComponent->OnComponentHit.AddDynamic(this, &ARequireKeyVolume::OnComponentHit);

}

// Called when the game starts or when spawned
void ARequireKeyVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARequireKeyVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARequireKeyVolume::ResetMessageScreen() {
	if (HUDMessage) {
		HUDMessage->SetVisibility(ESlateVisibility::Hidden);
		HUDMessage->RemoveFromParent();
	}
}

void ARequireKeyVolume::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	if (OtherActor) {
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		if (MainChar) {
			if (MainChar->GetKeyCount() > 0) {
				MainChar->SetKey(MainChar->GetKeyCount()-1);
				Destroy();
			}
			else {
				
			}
		}
	}
}

