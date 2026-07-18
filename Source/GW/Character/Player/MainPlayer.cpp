// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h" 

#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h" 

#include "Camera/CameraComponent.h" 

#include "Component/PlayerMoveComponent.h" 
#include "Component/PlayerSkillComponent.h" 

AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent")); 
	CameraComponent->SetupAttachment(GetMesh()); 
	CameraComponent->bUsePawnControlRotation = true; 

	MoveComponent = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("MoveComponent")); 
	SkillComponent = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("SkillComponent")); 
}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto IMC : IMC_List)
	{ 
		AddIMC(IMC, 0); 
	}
} 

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (auto EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{ 
		InputSetupDelegate.Broadcast(EIC); 
	}
}

bool AMainPlayer::Hit_Implementation(FAttackInfo& Info)
{
	return false;
}

UEnhancedInputLocalPlayerSubsystem* AMainPlayer::GetSubsystem()
{ 
	if (auto PC = Cast<APlayerController>(GetController()))
	{
		return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()); 
	} 
	return nullptr; 
} 

void AMainPlayer::AddIMC(UInputMappingContext* IMC, int Priority)
{ 
	if (auto Subsystem = GetSubsystem())
	{ 
		Subsystem->AddMappingContext(IMC, Priority); 
	}
}

void AMainPlayer::RemoveIMC(UInputMappingContext* IMC)
{ 
	if (auto Subsystem = GetSubsystem())
	{ 
		Subsystem->RemoveMappingContext(IMC); 
	}
}
