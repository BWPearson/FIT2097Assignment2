// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Door.h"
#include "Interactable.h"
#include "Interactable_Switch.generated.h"

UCLASS()
class MYPROJECT6_API AInteractable_Switch : public AInteractable
{
	GENERATED_BODY()
	
public:
	AInteractable_Switch();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeverStartPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LeverEndPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADoor* Door;
	


};
