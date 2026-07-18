// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../HitableCharacter.h"  
#include "MainPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputSetupDelegate, class UEnhancedInputComponent*); 

UCLASS()
class GW_API AMainPlayer : public AHitableCharacter 
{
	GENERATED_BODY()

public: 
	AMainPlayer(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override; 
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool Hit_Implementation(FAttackInfo& Info) override; 

	void AddIMC(class UInputMappingContext* IMC, int Priority);
	void RemoveIMC(class UInputMappingContext* IMC); 
	
	FInputSetupDelegate InputSetupDelegate; 

	UPROPERTY(EditAnywhere) 
	class UCameraComponent* CameraComponent; 

	UPROPERTY(EditAnywhere) 
	class UPlayerMoveComponent* MoveComponent; 
	UPROPERTY(EditAnywhere) 
	class UPlayerSkillComponent* SkillComponent; 

private: 
	class UEnhancedInputLocalPlayerSubsystem* GetSubsystem(); 

	UPROPERTY(EditAnywhere, Category = Input) 
	TArray<class UInputMappingContext*> IMC_List; 

};
