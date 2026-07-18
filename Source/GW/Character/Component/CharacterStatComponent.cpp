// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"

UCharacterStatComponent::UCharacterStatComponent()
{ 
	PrimaryComponentTick.bCanEverTick = true;

}

void UCharacterStatComponent::InitializeComponent()
{ 
	Super::InitializeComponent(); 

	CoreElemental = EElementalType::None;
	UsableElemental_List.Add(EElementalType::None);
	CoreElemental_Idx = 0; 

	// Test 
	UsableElemental_List.Add(EElementalType::Fire); 
	UsableElemental_List.Add(EElementalType::Water); 
	UsableElemental_List.Add(EElementalType::Wind); 
	UsableElemental_List.Add(EElementalType::Soil); 
	UsableElemental_List.Add(EElementalType::Ashes); 
}

void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterStatComponent::ChangeCoreElemental(int Idx)
{ 
	CoreElemental = UsableElemental_List[Idx]; 
	CoreElemental_Idx = Idx; 
}

