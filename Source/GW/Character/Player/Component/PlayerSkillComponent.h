// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerComponentBase.h" 
#include "InputAction.h" 
#include "PlayerSkillComponent.generated.h"

DECLARE_DELEGATE(FSkillDelegate)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GW_API UPlayerSkillComponent : public UPlayerComponentBase
{
	GENERATED_BODY()

public: 
	UPlayerSkillComponent(); 
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActiveWeaponBasicSkill(int Value); 

protected:
	virtual void InputSetup(class UEnhancedInputComponent* EIC) override;

private: 
	void OnDashStart(const FInputActionValue& Value); 
	void OnDashEnd(const FInputActionValue& Value); 
	void DashControl(float DeltaTime); 
	
	void OnManaOffset(const FInputActionValue& Value); 

	void OnManaControlStart(const FInputActionValue& Value); 
	void OnManaControlEnd(const FInputActionValue& Value); 
	void ManaCondense(); 

	void OnChangeElemental(const FInputActionValue& Value); 
	void OnSelectCoreElemental_Scroll(const FInputActionValue& Value); 
	void OnSelectCoreElemental(const FInputActionValue& Value); 

	void StartSkillCombination(); 
	void ExecuteSkill(); 

	void Dash(); 
	void ManaOffset(); 
	void ManaControl(); 

	bool bDashControl; 
	float DashControlTime; 
	UPROPERTY(EditAnywhere) 
	float DashControlMaxTime; 

	UPROPERTY(EditAnywhere) 
	float DashPower; 
	float DashPowerPercent; 
	UPROPERTY(EditAnywhere) 
	float DashHoldTime; 
	FTimerHandle DashHoldTimerHandle; 

	UPROPERTY(EditAnywhere) 
	float ManaOffsetTime; 
	FTimerHandle ManaOffsetHoldTimerHandle; 
	
	int CondensedMana; 
	UPROPERTY(EditAnywhere) 
	float ManaCondenseUnitTime; 
	UPROPERTY(EditAnywhere) 
	float ManaCondenseMoveSpeedSlowRate; 
	FTimerHandle ManaCondenseTimerHandle; 
	
	int ControllableMana; 
	UPROPERTY(EditAnywhere)
	float ControllableManaHoldTime;
	FTimerHandle ControllableManaTimerHandle;

	bool bActiveManaControl; 
	UPROPERTY(EditAnywhere) 
	float ManaControlStrengthBase; 
	UPROPERTY(EditAnywhere) 
	float ManaControlSpeedBase; 
	UPROPERTY(EditAnywhere) 
	float ManaControlManaResistanceBase; 
	UPROPERTY(EditAnywhere) 
	float ManaControlHoldTime; 
	FTimerHandle ManaControlTimerHandle; 
	

	UPROPERTY(EditAnywhere)
	class UInputMappingContext* IMC_SelectElemental; 
	UPROPERTY(EditAnywhere)
	TArray<class UInputAction*> IA_SelectElemental;
	
	bool bActiveSelectElemental; 
	int CurrentCoreElemental_Idx; 

	int BaseValue = 0b001000; 
	int SkillCombinationData; 

	bool bActiveSkillCombination; 
	UPROPERTY(EditAnywhere)
	float SkillCombinationTime;
	FTimerHandle SkillCombinationTimerHandle; 

	TArray<FSkillDelegate> Skill_List; 

};
