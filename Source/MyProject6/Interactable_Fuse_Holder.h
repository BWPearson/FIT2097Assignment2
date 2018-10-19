// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Interactable_Fuse_Holder.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT6_API AInteractable_Fuse_Holder : public AInteractable
{
	GENERATED_BODY()

	
public:
	AInteractable_Fuse_Holder();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPowered = false;
	
};
