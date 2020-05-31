// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Key.h"
#include "MainChar.h"

AItem_Key::AItem_Key() {
	value = 1;
}

void AItem_Key::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP BEGIN"));
	if (OtherActor) {
		AMainChar* MainCharRef = Cast<AMainChar>(OtherActor);
		if (MainCharRef) {
			MainCharRef->SetKey(MainCharRef->GetKeyCount() + value);
			Destroy();
		}
	}

}

void AItem_Key::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
