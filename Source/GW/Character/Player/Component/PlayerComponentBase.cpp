// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponentBase.h" 
#include "../MainPlayer.h" 

UPlayerComponentBase::UPlayerComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerComponentBase::InitializeComponent()
{
	Super::InitializeComponent(); 

	Player = Cast<AMainPlayer>(Character); 
	Player->InputSetupDelegate.AddUObject(this, &UPlayerComponentBase::InputSetup); 
}

void UPlayerComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
	Super::EndPlay(EndPlayReason); 

}

void UPlayerComponentBase::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
