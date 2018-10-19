// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Interactable.h"
#include "Door.h"
#include "Interactable_Fuse_Holder.h"
#include "Interactable_Powered_Switch.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT6_API AInteractable_Powered_Switch : public AInteractable
{
	GENERATED_BODY()
	
public:
	AInteractable_Powered_Switch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AInteractable_Fuse_Holder* FuseHolder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LeverStartPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector LeverEndPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ADoor* Door;
	
};
