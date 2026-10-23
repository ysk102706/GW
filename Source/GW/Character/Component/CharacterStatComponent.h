// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterComponentBase.h" 
#include "../../Utility/EnumUtility.h" 
#include "CharacterStatComponent.generated.h"

USTRUCT(BlueprintType)
struct FStat
{ 
	GENERATED_BODY() 

public: 
	UPROPERTY(EditAnywhere) 
	float Base = 0.0f; 
	UPROPERTY(EditAnywhere)
	float Add = 0.0f;
	UPROPERTY(EditAnywhere)
	float Mul = 1.0f;

	float Get() { return (Base + Add) * Mul; } 

};

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

	FStat Strength; 
	FStat Speed; 
	FStat ManaResistance; 

	EElementalType CoreElemental;
	TArray<EElementalType> UsableElemental_List;
	int CoreElemental_Idx; 

};
