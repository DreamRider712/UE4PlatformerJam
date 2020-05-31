// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = collisionBox;

	FlipBook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipBook->SetupAttachment(GetRootComponent());

	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
	collisionBox->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));
}

void AItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
}
