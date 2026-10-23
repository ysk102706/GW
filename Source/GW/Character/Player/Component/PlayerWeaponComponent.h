// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerComponentBase.h" 
#include "InputAction.h" 
#include "PlayerWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GW_API UPlayerWeaponComponent : public UPlayerComponentBase 
{
	GENERATED_BODY()

public:	
	UPlayerWeaponComponent();
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class AWeaponBase* GetWeapon() { return CurrentWeapon; }

protected:
	virtual void InputSetup(class UEnhancedInputComponent* EIC) override;

private: 
	void OnWeaponBasicSkill(const FInputActionValue& Value);
	void OnChangeWeapon(const FInputActionValue& Value); 

	UPROPERTY(EditAnywhere) 
	TArray<TSubclassOf<class AWeaponBase>> WeaponClass_List; 
	UPROPERTY() 
	TArray<class AWeaponBase*> WeaponObject_List; 

	UPROPERTY() 
	class AWeaponBase* CurrentWeapon; 

};
