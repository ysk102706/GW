// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_DELEGATE(FWeaponSkillDelegate) 

USTRUCT(BlueprintType) 
struct FCombinationSkillData 
{ 
	GENERATED_BODY() 

public: 
	UPROPERTY(EditAnywhere) 
	int Mana; 
	UPROPERTY(EditAnywhere)
	int Combination; 

	FWeaponSkillDelegate CombinationSkillDelegate; 

}; 

UCLASS()
class GW_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void TryCombinationSkill(int& Mana, int& Combination); 

protected: 
	virtual void Basic_1() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Basic 1")); }
	virtual void Basic_2() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Basic 2")); }
	virtual void Basic_3() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Basic 3")); }

	virtual void Combination_1() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Combination 1")); } 
	virtual void Combination_2() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Combination 2")); } 
	virtual void Combination_3() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Combination 3")); } 
	virtual void Combination_4() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Combination 4")); } 
	virtual void Combination_5() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Combination 5")); } 
	virtual void Combination_6() { GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Combination 6")); } 
	
private: 
	TMap<int, FWeaponSkillDelegate> BasicSkill_List;

	UPROPERTY(EditAnywhere) 
	TArray<FCombinationSkillData> CombinationSkillData_List; 

};
