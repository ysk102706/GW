// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BasicSkill_List.Add({ 0, FWeaponSkillDelegate() });
	BasicSkill_List.Add({ 1, FWeaponSkillDelegate() });
	BasicSkill_List.Add({ 2, FWeaponSkillDelegate() });

	CombinationSkillData_List.Add({}); 
	CombinationSkillData_List.Add({}); 
	CombinationSkillData_List.Add({}); 
	CombinationSkillData_List.Add({}); 
	CombinationSkillData_List.Add({}); 
	CombinationSkillData_List.Add({}); 
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	BasicSkill_List[0].BindUObject(this, &AWeaponBase::Basic_1); 
	BasicSkill_List[1].BindUObject(this, &AWeaponBase::Basic_2); 
	BasicSkill_List[2].BindUObject(this, &AWeaponBase::Basic_3); 
	
	CombinationSkillData_List[0].CombinationSkillDelegate.BindUObject(this, &AWeaponBase::Combination_1); 
	CombinationSkillData_List[1].CombinationSkillDelegate.BindUObject(this, &AWeaponBase::Combination_2); 
	CombinationSkillData_List[2].CombinationSkillDelegate.BindUObject(this, &AWeaponBase::Combination_3); 
	CombinationSkillData_List[3].CombinationSkillDelegate.BindUObject(this, &AWeaponBase::Combination_4); 
	CombinationSkillData_List[4].CombinationSkillDelegate.BindUObject(this, &AWeaponBase::Combination_5); 
	CombinationSkillData_List[5].CombinationSkillDelegate.BindUObject(this, &AWeaponBase::Combination_6); 
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::TryCombinationSkill(int& Mana, int& Combination)
{ 
	for (auto& A : CombinationSkillData_List) 
	{ 
		if (Mana < A.Mana) continue; 
		if ((Combination & A.Combination) == A.Combination) continue; 

		A.CombinationSkillDelegate.Execute(); 
		
		Mana = 0; 
		Combination &= ~(0b000111 | A.Combination); 

		return; 
	} 

	for (int i = 0; i < 3; i++)
	{ 
		if ((Combination & (1 << i)) == (1 << i))
		{
			BasicSkill_List[i].Execute(); 
			
			Combination &= 0b111000; 
			
			break; 
		} 
	}
}
