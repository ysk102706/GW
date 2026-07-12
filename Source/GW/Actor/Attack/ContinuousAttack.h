// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "ContinuousAttack.generated.h"

UCLASS()
class GW_API AContinuousAttack : public AAttackBase 
{
	GENERATED_BODY()
	
public:	
	AContinuousAttack(); 
	virtual void BeginPlay() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

protected: 
	virtual void OnDetectHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnMissHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<TScriptInterface<IHitable>> HitableObject_List; 

	FTimerHandle AttackTimerHandle; 

	UPROPERTY(EditAnywhere) 
	float AttackDelayTime; 
};
