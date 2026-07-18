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
}

void UPlayerMoveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
	Super::EndPlay(EndPlayReason); 

	GetWorld()->GetTimerManager().ClearTimer(LaunchHoldTimerHandle); 
}

void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	MoveSpeed_List.Add(EMoveState::Walk, WalkSpeed); 
	MoveSpeed_List.Add(EMoveState::Run, RunSpeed); 
	MoveSpeed_List.Add(EMoveState::Crouch, CrouchSpeed); 
	MoveSpeed_List.Add(EMoveState::Crawl, CrawlSpeed); 
	MoveSpeed_List.Add(EMoveState::Dash, DashSpeed); 

	ChangeMoveState(EMoveState::Walk); 
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

	EIC->BindAction(IA_List[int(EMovementType::Dash)], ETriggerEvent::Started, this, &UPlayerMoveComponent::OnDash); 
}

void UPlayerMoveComponent::OnMove(const FInputActionValue& Value)
{ 
	MoveDir = Value.Get<FVector2D>().GetSafeNormal(); 
	Player->AddMovementInput(Player->GetActorForwardVector(), MoveDir.X);
	Player->AddMovementInput(Player->GetActorRightVector(), MoveDir.Y);
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
	auto Hit = Player->CheckFloor();
	if (!Hit.bBlockingHit) return; 

	FVector F = Player->GetDirection(EDirectionType::Forward) * MoveDir.X; 
	FVector R = Player->GetDirection(EDirectionType::Right) * MoveDir.Y; 
	FVector Dir = F + R + FVector(0.0f, 0.0f, 0.025f); 
	
	LaunchPlayer(Dir, DashPower, DashHoldTime); 
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

void UPlayerMoveComponent::LaunchPlayer(FVector Dir, float Power, float HoldTime)
{ 
	ChangeMoveState(EMoveState::Dash);
	Player->LaunchCharacter(Dir * Power, false, false);

	GetWorld()->GetTimerManager().ClearTimer(LaunchHoldTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(LaunchHoldTimerHandle, [&]()
		{
			Player->GetCharacterMovement()->Velocity /= 5.0f;
			ChangeMoveState(EMoveState::Walk);
		}, HoldTime, false);
}