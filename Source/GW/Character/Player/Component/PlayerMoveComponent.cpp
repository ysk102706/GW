// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h" 

#include "../MainPlayer.h" 
#include "GameFramework/CharacterMovementComponent.h" 

#include "EnhancedInputComponent.h" 

#include "../../../Utility/EnumUtility.h" 

UPlayerMoveComponent::UPlayerMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerMoveComponent::InitializeComponent()
{ 
	Super::InitializeComponent(); 

	Player->bUseControllerRotationPitch = false;
	Player->bUseControllerRotationYaw = false;
	Player->bUseControllerRotationRoll = false; 

	MoveState = EMoveState::Walk; 
}

void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	MoveSpeed_List.Add(EMoveState::Walk, WalkSpeed); 
	MoveSpeed_List.Add(EMoveState::Run, RunSpeed); 
	MoveSpeed_List.Add(EMoveState::Crouch, CrouchSpeed); 
	MoveSpeed_List.Add(EMoveState::Crawl, CrawlSpeed); 
}

void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!MoveDir.IsNearlyZero())
	{ 
		FRotator Cur = Player->GetActorRotation(); 
		FRotator Tar = FRotator(0.0f, Player->GetControlRotation().Yaw, 0.0f); 

		FRotator Rot = FMath::RInterpTo(Cur, Tar, DeltaTime, 10.0f); 

		Player->SetActorRotation(Rot); 
	}
}

void UPlayerMoveComponent::InputSetup(UEnhancedInputComponent* EIC)
{ 
	EIC->BindAction(IA_List[int(EMovementType::Move)], ETriggerEvent::Triggered, this, &UPlayerMoveComponent::OnMove); 
	EIC->BindAction(IA_List[int(EMovementType::Move)], ETriggerEvent::Canceled, this, &UPlayerMoveComponent::OnMoveEnd); 
	EIC->BindAction(IA_List[int(EMovementType::Move)], ETriggerEvent::Completed, this, &UPlayerMoveComponent::OnMoveEnd); 

	EIC->BindAction(IA_List[int(EMovementType::Look)], ETriggerEvent::Triggered, this, &UPlayerMoveComponent::OnLook); 

	EIC->BindAction(IA_List[int(EMovementType::Jump)], ETriggerEvent::Started, this, &UPlayerMoveComponent::OnJump); 

	EIC->BindAction(IA_List[int(EMovementType::Crouch)], ETriggerEvent::Started, this, &UPlayerMoveComponent::OnCrouch); 
}

void UPlayerMoveComponent::OnMove(const FInputActionValue& Value)
{ 
	MoveDir = Value.Get<FVector2D>(); 
	Player->AddMovementInput(Player->GetActorForwardVector(), MoveDir.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), MoveDir.X);
}

void UPlayerMoveComponent::OnMoveEnd(const FInputActionValue& Value)
{ 
	MoveDir = FVector2D(0); 
}

void UPlayerMoveComponent::OnLook(const FInputActionValue& Value)
{ 
	FVector2D v = Value.Get<FVector2D>(); 
	Player->AddControllerPitchInput(-v.Y); 
	Player->AddControllerYawInput(v.X); 
}

void UPlayerMoveComponent::OnJump(const FInputActionValue& Value)
{ 
	Player->Jump(); 

	ChangeMoveState(EMoveState::Walk); 
	PrintMoveState(); 
}

void UPlayerMoveComponent::OnDash(const FInputActionValue& Value)
{
}

void UPlayerMoveComponent::OnCrouch(const FInputActionValue& Value)
{ 
	switch (MoveState)
	{
	case EMoveState::Walk: 
		ChangeMoveState(EMoveState::Crouch); 
		break; 
	case EMoveState::Crouch: 
		ChangeMoveState(EMoveState::Crawl); 
		break; 
	case EMoveState::Crawl:
		ChangeMoveState(EMoveState::Walk); 
		break; 
	}

	PrintMoveState(); 
}

void UPlayerMoveComponent::PrintMoveState()
{ 
	int v = int(MoveState); 
	FString E = StaticEnum<EMoveState>()->GetNameStringByValue(v); 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, *E); 
}

void UPlayerMoveComponent::ChangeMoveState(EMoveState State)
{ 
	MoveState = State; 
	Player->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed_List[State]; 
}
