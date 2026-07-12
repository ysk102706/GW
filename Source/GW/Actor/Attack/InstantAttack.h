// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "InstantAttack.generated.h"

UCLASS()
class GW_API AInstantAttack : public AAttackBase 
{
	GENERATED_BODY()
	
public:	 
	AInstantAttack(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

protected: 
	virtual void OnDetectHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
