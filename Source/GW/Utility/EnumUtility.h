// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumUtility.generated.h" 

UENUM(BlueprintType) 
enum class EDirectionType : uint8
{
	Forward, 
	Back, 
	Right, 
	Left, 
	Up, 
	Down 
}; 

UENUM(BlueprintType) 
enum class ECharacterPointType : uint8
{ 
	Center, 
	Chest, 
	Head, 
	Foot, 

	Count 
}; 
 
UENUM(BlueprintType) 
enum class ECharacterType : uint8
{ 
	Player, 
	Enemy, 
	Etc 
}; 

UENUM() 
enum class EColliderType : uint8
{ 
	Box, 
	Sphere, 
	Capsule 
}; 

UENUM() 
enum class EMovementType : uint8
{ 
	Move, 
	Look, 
	Jump, 
	Crouch, 
	Dash 
}; 

UENUM(BlueprintType) 
enum class EMoveState : uint8
{ 
	Walk, 
	Run, 
	Crouch, 
	Crawl 
}; 