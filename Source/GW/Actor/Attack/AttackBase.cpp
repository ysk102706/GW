// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBase.h" 
#include "Components/BoxComponent.h" 
#include "Components/SphereComponent.h" 
#include "Components/CapsuleComponent.h" 

AAttackBase::AAttackBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent"); 
	BoxComponent->SetupAttachment(RootComponent); 

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent"); 
	SphereComponent->SetupAttachment(RootComponent); 
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent"); 
	CapsuleComponent->SetupAttachment(RootComponent); 

	Collider_List.Add(EColliderType::Box, BoxComponent); 
	Collider_List.Add(EColliderType::Sphere, SphereComponent); 
	Collider_List.Add(EColliderType::Capsule, CapsuleComponent); 
}

void AAttackBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetCollider(ColliderType); 

	AttackComponent->SetCollisionProfileName("Attack"); 
	AttackComponent->OnComponentBeginOverlap.AddDynamic(this, &AAttackBase::OnDetectHitableObject); 
	AttackComponent->OnComponentEndOverlap.AddDynamic(this, &AAttackBase::OnMissHitableObject); 

	// test 
	SetAttackInfo(10); 
}

void AAttackBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

void AAttackBase::SetAttackInfo(float Damage)
{ 
	AttackInfo.Damage = Damage; 
}

void AAttackBase::SetCollider(EColliderType Type)
{ 
	AttackComponent = Collider_List[Type]; 

	for (auto Collider : Collider_List)
	{ 
		if (Collider.Key == Type)
		{ 
			Collider.Value->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
		} 
		else
		{ 
			Collider.Value->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		} 
	}
}

void AAttackBase::Attack(AActor* HitableObject)
{ 
	IHitable::Execute_Hit(HitableObject, AttackInfo); 
}
