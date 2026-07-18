// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Component/CharacterComponentBase.h" 
#include "PlayerComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GW_API UPlayerComponentBase : public UCharacterComponentBase
{
	GENERATED_BODY()

public:
	UPlayerComponentBase();
	virtual void InitializeComponent() override; 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; 
	virtual void BeginPlay() override; 
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	virtual void InputSetup(class UEnhancedInputComponent* EIC) {};

	UPROPERTY() 
	class AMainPlayer* Player; 

	UPROPERTY(EditAnywhere) 
	TArray<class UInputAction*> IA_List; 

};
