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
		auto Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass_List[i]); 
		Weapon->SetPlayer(Player); 

		WeaponObject_List.Add(Weapon); 
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

	EIC->BindAction(BasicIA_List[int(EWeaponActionType::BasicSkill)], ETriggerEvent::Started, this, &UPlayerWeaponComponent::OnWeaponBasicSkillStart);
	EIC->BindAction(BasicIA_List[int(EWeaponActionType::BasicSkill)], ETriggerEvent::Canceled, this, &UPlayerWeaponComponent::OnWeaponBasicSkillEnd);
	EIC->BindAction(BasicIA_List[int(EWeaponActionType::BasicSkill)], ETriggerEvent::Completed, this, &UPlayerWeaponComponent::OnWeaponBasicSkillEnd);
	EIC->BindAction(BasicIA_List[int(EWeaponActionType::AttackDirection)], ETriggerEvent::Triggered, this, &UPlayerWeaponComponent::OnWeaponAttackDirection);
	EIC->BindAction(BasicIA_List[int(EWeaponActionType::Change)], ETriggerEvent::Started, this, &UPlayerWeaponComponent::OnChangeWeapon); 
}

void UPlayerWeaponComponent::OnWeaponBasicSkillStart(const FInputActionValue& Value)
{ 
	float V = Value.Get<float>(); 
	Player->SkillComponent->ActiveWeaponBasicSkill(1 << int(V)); 

	AttackDirection = FVector2D(0); 
}

void UPlayerWeaponComponent::OnWeaponAttackDirection(const FInputActionValue& Value)
{ 
	FVector2D V = Value.Get<FVector2D>(); 
	AttackDirection += V; 
}

void UPlayerWeaponComponent::OnWeaponBasicSkillEnd(const FInputActionValue& Value)
{ 
	AttackDirection.Normalize(); 
	CurrentWeapon->SetMouseDirection(AttackDirection); 

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f %f"), AttackDirection.X, AttackDirection.Y));
}

void UPlayerWeaponComponent::OnChangeWeapon(const FInputActionValue& Value)
{ 
	float V = Value.Get<float>(); 
	CurrentWeapon = WeaponObject_List[int(V)]; 
}

