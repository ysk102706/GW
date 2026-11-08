// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI_Player.generated.h"

UCLASS()
class GW_API UAI_Player : public UAnimInstance
{
	GENERATED_BODY()
	
public: 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetAttackDirection(FVector2D Value) { AttackDirection = Value; }

private: 
	UPROPERTY() 
	class AMainPlayer* Player; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector Velocity; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Speed; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Direction; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bFalling; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector2D AttackDirection; 
	
}; 
