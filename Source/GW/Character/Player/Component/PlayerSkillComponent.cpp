// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h" 

#include "../MainPlayer.h" 
#include "Camera/CameraComponent.h" 
#include "PlayerMovecomponent.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "../../Component/CharacterStatComponent.h" 

#include "EnhancedInputComponent.h" 
#include "../../../Utility/EnumUtility.h" 

UPlayerSkillComponent::UPlayerSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerSkillComponent::InitializeComponent()
{ 
	Super::InitializeComponent(); 

}

void UPlayerSkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
	Super::EndPlay(EndPlayReason);
	
	GetWorld()->GetTimerManager().ClearTimer(DashHoldTimerHandle); 

	GetWorld()->GetTimerManager().ClearTimer(ManaOffsetHoldTimerHandle); 
	
	GetWorld()->GetTimerManager().ClearTimer(ManaCondenseTimerHandle); 
}

void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDashControl) DashControl(DeltaTime); 
}

void UPlayerSkillComponent::InputSetup(UEnhancedInputComponent* EIC)
{ 
	Super::InputSetup(EIC); 

	EIC->BindAction(IA_List[int(ESkillType::Dash)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnDashStart); 
	EIC->BindAction(IA_List[int(ESkillType::Dash)], ETriggerEvent::Canceled, this, &UPlayerSkillComponent::OnDashEnd); 
	EIC->BindAction(IA_List[int(ESkillType::Dash)], ETriggerEvent::Completed, this, &UPlayerSkillComponent::OnDashEnd); 

	EIC->BindAction(IA_List[int(ESkillType::ManaOffset)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnManaOffset); 
	
	EIC->BindAction(IA_List[int(ESkillType::ChangeElemental)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnChangeElemental); 
	EIC->BindAction(IA_SelectElemental[int(EScrollType::Scroll)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnSelectCoreElemental_Scroll); 
	EIC->BindAction(IA_SelectElemental[int(EScrollType::Click)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnSelectCoreElemental);

	EIC->BindAction(IA_List[int(ESkillType::ManaControl)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnManaControlStart); 
	EIC->BindAction(IA_List[int(ESkillType::ManaControl)], ETriggerEvent::Canceled, this, &UPlayerSkillComponent::OnManaControlEnd); 
	EIC->BindAction(IA_List[int(ESkillType::ManaControl)], ETriggerEvent::Completed, this, &UPlayerSkillComponent::OnManaControlEnd); 
}

void UPlayerSkillComponent::OnDashStart(const FInputActionValue& Value)
{ 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Dash Start")); 

	bDashControl = true; 
	DashControlTime = 0.0f; 
	DashPowerPercent = 0.0f; 
}

void UPlayerSkillComponent::OnDashEnd(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Dash End %f"), DashPowerPercent)); 

	bDashControl = false; 

	FVector Dir = Player->CameraComponent->GetForwardVector(); 
	auto Hit = Player->CheckFloor(); 
	if (Hit.bBlockingHit)
	{ 
		float Angle = FMath::RadiansToDegrees(FMath::Acos(Dir.Dot(Hit.ImpactNormal))); 
		if (Angle > 87.5f)
		{ 
			Dir = Player->CameraComponent->GetRightVector().Cross(Hit.ImpactNormal).GetSafeNormal(); 
			Dir += FVector(0, 0, 0.075f);
		} 
	} 

	Player->MoveComponent->LaunchPlayer(Dir, DashPower * (DashPowerPercent * 0.5f + 0.5f), DashHoldTime); 
}

void UPlayerSkillComponent::DashControl(float DeltaTime)
{ 
	DashControlTime += DeltaTime; 
	DashControlTime = FMath::Min(DashControlTime, DashControlMaxTime); 

	DashPowerPercent = DashControlTime / DashControlMaxTime; 
}

void UPlayerSkillComponent::OnManaOffset(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Mana Offset")); 

	Player->StatComponent->ManaResistance += 10000; 
	GetWorld()->GetTimerManager().ClearTimer(ManaOffsetHoldTimerHandle); 
	GetWorld()->GetTimerManager().SetTimer(ManaOffsetHoldTimerHandle, [&]()
		{ 
			Player->StatComponent->ManaResistance -= 10000; 
		}, ManaOffsetTime, false); 
}

void UPlayerSkillComponent::OnChangeElemental(const FInputActionValue& Value)
{ 
	if (bActiveSelectElemental) return; 

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Change Elemental Start")); 
	
	Player->AddIMC(IMC_SelectElemental, 1); 

	bActiveSelectElemental = true; 
	CurrentCoreElemental_Idx = Player->StatComponent->CoreElemental_Idx; 
}

void UPlayerSkillComponent::OnSelectCoreElemental_Scroll(const FInputActionValue& Value)
{ 
	float v = Value.Get<float>(); 
	CurrentCoreElemental_Idx -= FMath::Sign(v); 

	int Count = Player->StatComponent->UsableElemental_List.Num(); 
	if (CurrentCoreElemental_Idx < 0) CurrentCoreElemental_Idx = Count - 1; 

	CurrentCoreElemental_Idx %= Count; 

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Current Elemental %d"), CurrentCoreElemental_Idx)); 
}

void UPlayerSkillComponent::OnSelectCoreElemental(const FInputActionValue& Value)
{ 
	bActiveSelectElemental = false; 
	Player->StatComponent->ChangeCoreElemental(CurrentCoreElemental_Idx); 

	Player->RemoveIMC(IMC_SelectElemental); 

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Current Elemental End")); 
}

void UPlayerSkillComponent::OnManaControlStart(const FInputActionValue& Value)
{ 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Mana Condense Start")); 

	CondensedMana = 0; 
	Player->StatComponent->Speed /= ManaCondenseMoveSpeedSlowRate; 

	ManaCondense(); 
}

void UPlayerSkillComponent::OnManaControlEnd(const FInputActionValue& Value) 
{ 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Mana Condense End")); 

	GetWorld()->GetTimerManager().ClearTimer(ManaCondenseTimerHandle); 

	ControllableMana = CondensedMana; 
	Player->StatComponent->Speed *= ManaCondenseMoveSpeedSlowRate; 

	if (bActiveManaControl) return; 

	bActiveManaControl = true; 
	
	Player->StatComponent->Strength *= FMath::Pow(ManaControlStrengthBase, ControllableMana);
	Player->StatComponent->Speed *= FMath::Pow(ManaControlSpeedBase, ControllableMana);
	Player->StatComponent->ManaResistance *= FMath::Pow(ManaControlManaResistanceBase, ControllableMana);

	GetWorld()->GetTimerManager().ClearTimer(ManaControlTimerHandle); 
	GetWorld()->GetTimerManager().SetTimer(ManaControlTimerHandle, [&]()
		{ 
			Player->StatComponent->Strength /= FMath::Pow(ManaControlStrengthBase, ControllableMana);
			Player->StatComponent->Speed /= FMath::Pow(ManaControlSpeedBase, ControllableMana);
			Player->StatComponent->ManaResistance /= FMath::Pow(ManaControlManaResistanceBase, ControllableMana);

			bActiveManaControl = false; 
		}, ManaControlHoldTime, false);
}

void UPlayerSkillComponent::ManaCondense()
{
	if (CondensedMana > 9) return; 

	GetWorld()->GetTimerManager().SetTimer(ManaCondenseTimerHandle, [&]()
		{
			CondensedMana++; 
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Condensed Mana %d"), CondensedMana)); 

			ManaCondense(); 
		}, FMath::Pow(ManaCondenseUnitTime, CondensedMana) * 0.25f, false);
}
