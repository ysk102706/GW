// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantAttack.h"

AInstantAttack::AInstantAttack()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AInstantAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInstantAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInstantAttack::OnDetectHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
	if (OtherActor->Implements<UHitable>())
	{ 
		Attack(OtherActor); 
	}
}

