// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerComponentBase.h" 
#include "InputAction.h" 
#include "../../../Utility/EnumUtility.h" 
#include "PlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GW_API UPlayerMoveComponent : public UPlayerComponentBase 
{
	GENERATED_BODY()

public:	
	UPlayerMoveComponent();
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
	virtual void BeginPlay() override; 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ChangeMoveState(EMoveState State); 

	void LaunchPlayer(FVector Dir, float Power, float HoldTime); 

protected: 
	virtual void InputSetup(class UEnhancedInputComponent* EIC) override;

private: 
	void OnMove(const FInputActionValue& Value); 
	void OnMoveEnd(const FInputActionValue& Value); 
	void OnLook(const FInputActionValue& Value); 
	void OnJump(const FInputActionValue& Value); 
	void OnDash(const FInputActionValue& Value); 
	void OnCrouch(const FInputActionValue& Value); 

	void PrintMoveState(); 

	FVector2D MoveDir; 
	EMoveState MoveState; 
	TMap<EMoveState, float> MoveSpeed_List; 

	UPROPERTY(EditAnywhere, Category = Move)
	float WalkSpeed; 
	UPROPERTY(EditAnywhere, Category = Move)
	float RunSpeed; 
	UPROPERTY(EditAnywhere, Category = Move) 
	float CrouchSpeed; 
	UPROPERTY(EditAnywhere, Category = Move)
	float CrawlSpeed; 
	UPROPERTY(EditAnywhere, Category = Move)
	float DashSpeed; 

	FTimerHandle LaunchHoldTimerHandle; 

	UPROPERTY(EditAnywhere, Category = Move)
	float DashPower;
	UPROPERTY(EditAnywhere, Category = Move)
	float DashHoldTime; 
	
};
