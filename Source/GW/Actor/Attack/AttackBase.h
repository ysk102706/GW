// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "../../Interface/Hitable.h" 
#include "../../Utility/EnumUtility.h" 
#include "AttackBase.generated.h"

UCLASS()
class GW_API AAttackBase : public AActor
{
	GENERATED_BODY()
	
public: 
	AAttackBase(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	void SetAttackInfo(float Damage); 
	void SetCollider(EColliderType Type); 

protected: 
	UFUNCTION() 
	virtual void OnDetectHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}; 
	UFUNCTION() 
	virtual void OnMissHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {};

	void Attack(AActor* HitableObject); 

	FAttackInfo AttackInfo; 

	UPROPERTY(EditAnywhere) 
	UPrimitiveComponent* AttackComponent; 

private: 
	UPROPERTY() 
	class UBoxComponent* BoxComponent; 
	UPROPERTY()
	class USphereComponent* SphereComponent;
	UPROPERTY()
	class UCapsuleComponent* CapsuleComponent; 

	UPROPERTY(EditAnywhere) 
	EColliderType ColliderType; 
	UPROPERTY() 
	TMap<EColliderType, UPrimitiveComponent*> Collider_List; 

};
