// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponentBase.h" 
#include "../CharacterBase.h" 

UCharacterComponentBase::UCharacterComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true; 
}

void UCharacterComponentBase::InitializeComponent()
{ 
	Super::InitializeComponent(); 

	Character = Cast<ACharacterBase>(GetOwner()); 
}

void UCharacterComponentBase::BeginPlay()
{
	Super::BeginPlay();

}


void UCharacterComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

