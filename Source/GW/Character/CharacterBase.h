// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h" 
#include "../Utility/EnumUtility.h" 
#include "CharacterBase.generated.h"

UCLASS()
class GW_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetDirection(EDirectionType Type); 
	int CheckDirection(EDirectionType Type, EDirectionType Target); 
	
	FHitResult LineTraceByCharacter(ECharacterPointType PointType, FVector Direction, float Length, ECollisionChannel Channel);
	FHitResult CheckFloor(); 

	UPROPERTY(EditAnywhere) 
	class UCharacterStatComponent* StatComponent; 

private: 
	UPROPERTY(EditAnywhere) 
	TArray<USceneComponent*> CharacterPoint_List; 

	ECharacterType CharacterType; 

};
