// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_PlayerAttackEnd.h" 

#include "../MainPlayer.h" 
#include "AI_Player.h"  

void UAN_PlayerAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{ 
	if (auto Player = Cast<AMainPlayer>(MeshComp->GetOwner()))
	{ 
		Player->GetAnimInstance<UAI_Player>()->SetAttackDirection(FVector2D(0)); 
	}
}
