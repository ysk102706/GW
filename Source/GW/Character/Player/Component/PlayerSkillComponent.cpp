// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h" 

#include "../MainPlayer.h" 
#include "Camera/CameraComponent.h" 
#include "PlayerMoveComponent.h" 
#include "PlayerWeaponComponent.h" 
#include "../../Component/CharacterStatComponent.h" 

#include "EnhancedInputComponent.h" 
#include "../../../Utility/EnumUtility.h" 

#include "../../../Actor/Weapon/WeaponBase.h" 

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
	GetWorld()->GetTimerManager().ClearTimer(ControllableManaTimerHandle); 
	GetWorld()->GetTimerManager().ClearTimer(ManaControlTimerHandle); 
	GetWorld()->GetTimerManager().ClearTimer(SkillCombinationTimerHandle); 
} 

void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay(); 

	Skill_List.Add(FSkillDelegate()); 
	Skill_List.Add(FSkillDelegate()); 
	Skill_List.Add(FSkillDelegate()); 

	Skill_List[0].BindUObject(this, &UPlayerSkillComponent::Dash); 
	Skill_List[1].BindUObject(this, &UPlayerSkillComponent::ManaOffset); 
	Skill_List[2].BindUObject(this, &UPlayerSkillComponent::ManaControl); 
}

void UPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bDashControl) DashControl(DeltaTime); 
}

void UPlayerSkillComponent::InputSetup(UEnhancedInputComponent* EIC)
{ 
	Super::InputSetup(EIC); 

	EIC->BindAction(BasicIA_List[int(ESkillType::Dash)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnDashStart); 
	EIC->BindAction(BasicIA_List[int(ESkillType::Dash)], ETriggerEvent::Canceled, this, &UPlayerSkillComponent::OnDashEnd); 
	EIC->BindAction(BasicIA_List[int(ESkillType::Dash)], ETriggerEvent::Completed, this, &UPlayerSkillComponent::OnDashEnd); 

	EIC->BindAction(BasicIA_List[int(ESkillType::ManaOffset)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnManaOffset); 
	
	EIC->BindAction(BasicIA_List[int(ESkillType::ManaControl)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnManaControlStart); 
	EIC->BindAction(BasicIA_List[int(ESkillType::ManaControl)], ETriggerEvent::Canceled, this, &UPlayerSkillComponent::OnManaControlEnd); 
	EIC->BindAction(BasicIA_List[int(ESkillType::ManaControl)], ETriggerEvent::Completed, this, &UPlayerSkillComponent::OnManaControlEnd); 

	EIC->BindAction(BasicIA_List[int(ESkillType::ChangeElemental)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnChangeElemental); 
	EIC->BindAction(IA_SelectElemental[int(EScrollType::Scroll)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnSelectCoreElemental_Scroll); 
	EIC->BindAction(IA_SelectElemental[int(EScrollType::Click)], ETriggerEvent::Started, this, &UPlayerSkillComponent::OnSelectCoreElemental);
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

	SkillCombinationData |= BaseValue << int(ESkillType::Dash); 
	StartSkillCombination(); 
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

	SkillCombinationData |= BaseValue << int(ESkillType::ManaOffset);
	StartSkillCombination(); 
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
	
	Player->StatComponent->Speed *= ManaCondenseMoveSpeedSlowRate; 

	SkillCombinationData |= BaseValue << int(ESkillType::ManaControl);
	
	GetWorld()->GetTimerManager().ClearTimer(ControllableManaTimerHandle); 
	GetWorld()->GetTimerManager().SetTimer(ControllableManaTimerHandle, [&]()
		{ 
			SkillCombinationData &= ~(BaseValue << int(ESkillType::ManaControl)); 

			ManaControl(); 
		}, ControllableManaHoldTime, false); 
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

void UPlayerSkillComponent::ActiveWeaponBasicSkill(int Value)
{ 
	SkillCombinationData |= Value; 
	StartSkillCombination(); 
}

void UPlayerSkillComponent::StartSkillCombination()
{ 
	if (bActiveSkillCombination) return; 

	bActiveSkillCombination = true; 
	GetWorld()->GetTimerManager().SetTimer(SkillCombinationTimerHandle, this, &UPlayerSkillComponent::ExecuteSkill, SkillCombinationTime, false); 
} 

void UPlayerSkillComponent::ExecuteSkill()
{ 
	Player->WeaponComponent->GetWeapon()->TryCombinationSkill(CondensedMana, SkillCombinationData); 

	bActiveSkillCombination = false; 

	for (int i = 0; i < int(ESkillType::ManaOffset); i++)
	{ 
		if ((SkillCombinationData & (BaseValue << i)) == (BaseValue << i))
		{ 
			Skill_List[i].Execute(); 
		}
	} 

	SkillCombinationData &= BaseValue << int(ESkillType::ManaControl); 
}

void UPlayerSkillComponent::Dash()
{ 
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

void UPlayerSkillComponent::ManaOffset()
{ 
	Player->StatComponent->ManaResistance += 10000;
	GetWorld()->GetTimerManager().ClearTimer(ManaOffsetHoldTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ManaOffsetHoldTimerHandle, [&]()
		{
			Player->StatComponent->ManaResistance -= 10000;
		}, ManaOffsetTime, false); 
}

void UPlayerSkillComponent::ManaControl()
{ 
	if (bActiveManaControl) return;

	bActiveManaControl = true; 
	ControllableMana = CondensedMana; 

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
