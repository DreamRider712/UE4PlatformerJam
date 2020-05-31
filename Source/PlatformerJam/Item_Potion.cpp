// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Potion.h"
#include "MainChar.h"

//Constrctor
AItem_Potion::AItem_Potion() {
	value = 0.f;
}

void AItem_Potion::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));
	if (OtherActor) {
		AMainChar* MainCharRef = Cast<AMainChar>(OtherActor);
		if(MainCharRef){
			MainCharRef->SetHealth(MainCharRef->GetHealth() + value);
			Destroy();
		}
	}

}

void AItem_Potion::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
