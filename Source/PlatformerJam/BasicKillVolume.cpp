// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicKillVolume.h"
#include "Components/BoxComponent.h"
#include "MainChar.h"
#include "EnemyBase.h"

// Sets default values
ABasicKillVolume::ABasicKillVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABasicKillVolume::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABasicKillVolume::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABasicKillVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicKillVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicKillVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		//Setup logic to kill either player or enemy upon falling on the volume
		AMainChar* MainChar = Cast<AMainChar>(OtherActor);
		AEnemyBase* EnemyChar = Cast<AEnemyBase>(OtherActor);
		if (MainChar) {
			MainChar->Death();
		}
		if (EnemyChar) {
			EnemyChar->Death();
		}
	}
}

void ABasicKillVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}
