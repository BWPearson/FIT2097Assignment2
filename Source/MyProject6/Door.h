// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class MYPROJECT6_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DoorInitialLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DoorStartPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DoorEndPos;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
