// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponComponent.h"

#include "EnhancedInputComponent.h" 
#include "../../../Utility/EnumUtility.h" 

#include "../../../Actor/Weapon/WeaponBase.h" 

#include "../MainPlayer.h" 
#include "PlayerSkillComponent.h" 

UPlayerWeaponComponent::UPlayerWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerWeaponComponent::InitializeComponent()
{ 
	Super::InitializeComponent(); 

}

void UPlayerWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < WeaponClass_List.Num(); i++)
	{ 
		WeaponObject_List.Add(GetWorld()->SpawnActor<AWeaponBase>(WeaponClass_List[i]));
	} 

	CurrentWeapon = WeaponObject_List[0]; 
}

void UPlayerWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerWeaponComponent::InputSetup(UEnhancedInputComponent* EIC)
{ 
	Super::InputSetup(EIC);

	EIC->BindAction(BasicIA_List[int(EWeaponActionType::BasicSkill)], ETriggerEvent::Started, this, &UPlayerWeaponComponent::OnWeaponBasicSkill);
	EIC->BindAction(BasicIA_List[int(EWeaponActionType::Change)], ETriggerEvent::Started, this, &UPlayerWeaponComponent::OnChangeWeapon); 
}

void UPlayerWeaponComponent::OnWeaponBasicSkill(const FInputActionValue& Value)
{ 
	float V = Value.Get<float>(); 
	
	Player->SkillComponent->ActiveWeaponBasicSkill(1 << int(V)); 
}

void UPlayerWeaponComponent::OnChangeWeapon(const FInputActionValue& Value)
{ 
	float V = Value.Get<float>(); 
	CurrentWeapon = WeaponObject_List[int(V)]; 
}

