// Fill out your copyright notice in the Description page of Project Settings.


#include "HitableCharacter.h"

AHitableCharacter::AHitableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AHitableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHitableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AHitableCharacter::Hit_Implementation(FAttackInfo& Info)
{ 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f"), Info.Damage)); 

	return true; 
}

