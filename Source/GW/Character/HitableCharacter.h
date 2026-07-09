// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h" 
#include "../Interface/Hitable.h" 
#include "HitableCharacter.generated.h"

UCLASS()
class GW_API AHitableCharacter : public ACharacterBase, public IHitable 
{
	GENERATED_BODY()

public:
	AHitableCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool Hit_Implementation(FAttackInfo& Info) override; 

};
