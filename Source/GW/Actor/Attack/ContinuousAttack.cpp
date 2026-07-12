// Fill out your copyright notice in the Description page of Project Settings.


#include "ContinuousAttack.h"

AContinuousAttack::AContinuousAttack()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AContinuousAttack::BeginPlay()
{
	Super::BeginPlay();
	
	AttackDelayTime = 0.2f; 

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [&]()
		{
			for (TScriptInterface<IHitable> Object : HitableObject_List)
			{
				Attack(Cast<AActor>(Object.GetObject()));
			}
		}, AttackDelayTime, true); 
}

void AContinuousAttack::EndPlay(const EEndPlayReason::Type EndPlayReason)
{ 
	Super::EndPlay(EndPlayReason); 

	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle); 
}

void AContinuousAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AContinuousAttack::OnDetectHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UHitable>())
	{ 
		TScriptInterface<IHitable> Interface; 
		Interface.SetObject(OtherActor); 
		Interface.SetInterface(Cast<IHitable>(OtherActor)); 

		HitableObject_List.Add(Interface); 
	}
} 

void AContinuousAttack::OnMissHitableObject(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UHitable>())
	{
		TScriptInterface<IHitable> Interface;
		Interface.SetObject(OtherActor);
		Interface.SetInterface(Cast<IHitable>(OtherActor));

		HitableObject_List.Remove(Interface);
	}
}