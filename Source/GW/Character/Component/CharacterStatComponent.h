// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterComponentBase.h" 
#include "../../Utility/EnumUtility.h" 
#include "CharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GW_API UCharacterStatComponent : public UCharacterComponentBase 
{
	GENERATED_BODY()

public: 
	UCharacterStatComponent();
	virtual void InitializeComponent() override; 
	virtual void BeginPlay() override; 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeCoreElemental(int Idx); 

	float Strength; 
	float Speed; 
	float ManaResistance; 

	EElementalType CoreElemental;
	TArray<EElementalType> UsableElemental_List;
	int CoreElemental_Idx; 

};
