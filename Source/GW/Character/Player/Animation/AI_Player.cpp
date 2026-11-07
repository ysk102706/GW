// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Player.h" 
#include "../MainPlayer.h" 
#include "GameFramework/CharacterMovementComponent.h" 

void UAI_Player::NativeInitializeAnimation()
{ 
	Super::NativeInitializeAnimation(); 

	Player = Cast<AMainPlayer>(GetOwningActor()); 
}

void UAI_Player::NativeUpdateAnimation(float DeltaSeconds)
{ 
	Super::NativeUpdateAnimation(DeltaSeconds); 

	if (!Player->IsValidLowLevelFast()) return; 

	auto MovementComponent = Player->GetMovementComponent(); 
	FVector Velocity = MovementComponent->Velocity;
	Speed = Velocity.Size2D(); 

	FRotator Rot = Player->GetActorRotation();
	Direction = CalculateDirection(Velocity, Rot); 

	bFalling = MovementComponent->IsFalling(); 
}
