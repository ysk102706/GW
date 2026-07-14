// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h" 
#include "GameFramework/CharacterMovementComponent.h" 

ACharacterBase::ACharacterBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < int(ECharacterPointType::Count); i++)
	{ 
		FString E = StaticEnum<ECharacterPointType>()->GetNameStringByValue(i); 
		FString N = FString::Printf(TEXT("%sPoint"), *E);
		CharacterPoint_List.Add(CreateDefaultSubobject<USceneComponent>(*N)); 
	}
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FVector ACharacterBase::GetDirection(EDirectionType Type)
{ 
	FVector D1 = FVector(1, 0, 0) * CheckDirection(Type, EDirectionType::Forward); 
	FVector D2 = FVector(0, 1, 0) * CheckDirection(Type, EDirectionType::Right);
	FVector D3 = FVector(0, 0, 1) * CheckDirection(Type, EDirectionType::Up);
	
	return D1 + D2 + D3; 
} 

int ACharacterBase::CheckDirection(EDirectionType Type, EDirectionType Target)
{ 
	int Comp = Type == Target || Type == EDirectionType(int(Target) + 1); 
	return Comp * FMath::Pow(-1.0f, int(Type) % 2) ;
}

FHitResult ACharacterBase::LineTraceByCharacter(ECharacterPointType PointType, FVector Direction, float Length, ECollisionChannel Channel)
{ 
	FHitResult Hit; 
	FVector Start = CharacterPoint_List[int(PointType)]->GetComponentLocation(); 
	FVector End = Start + Direction * Length; 
	FCollisionQueryParams CQP; 
	CQP.AddIgnoredActor(this); 
	
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, CQP); 

	return Hit;
}

