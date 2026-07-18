// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GW_API UCharacterComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterComponentBase(); 
	virtual void InitializeComponent() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
	virtual void BeginPlay() override; 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected: 
	UPROPERTY() 
	class ACharacterBase* Character; 
};
