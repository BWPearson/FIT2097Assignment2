// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProject6Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Interactable.h"
#include "Interactable_Fuse.h"
#include "Interactable_Fuse_Holder.h"
#include "Interactable_Switch.h"
#include "Interactable_Powered_Switch.h"

//////////////////////////////////////////////////////////////////////////
// AMyProject6Character

AMyProject6Character::AMyProject6Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	Health = 100;
	MaxHealth = Health;
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject6Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProject6Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProject6Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProject6Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProject6Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProject6Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProject6Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProject6Character::OnResetVR);

	//*****************************************************
	//  MY CODE
	//*****************************************************
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMyProject6Character::OnAction);
}


void AMyProject6Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProject6Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyProject6Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyProject6Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProject6Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyProject6Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyProject6Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//******************************************************
// MY CODE, via: https://www.youtube.com/watch?v=D6G8_PU8ZFY
//******************************************************
void AMyProject6Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult OutHit;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector Forward = FollowCamera->GetForwardVector();
	FVector End = (Start + (Forward * 750.0f)); //float is length of line or detection distance

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), (Start + (Forward * 300.0f)), End, FColor::White, false, 1, 0, 0.2f); //float operation is start point of line in 3rd person
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			if (GEngine)
			{
				if (OutHit.GetActor()->IsA(AInteractable::StaticClass()))
				{
					CurrentHit = OutHit;
					AInteractable* Interactable = Cast<AInteractable>(OutHit.GetActor());
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%s"), *Interactable->DisplayText));
				}
				else {
					CurrentHit.Reset();
				}
			}
		}
	}
	else {
		CurrentHit.Reset();
	}
}

void AMyProject6Character::OnAction() {
	if (CurrentHit.GetActor()) {
		if (CurrentHit.GetActor()->IsA(AInteractable::StaticClass())) {
			AInteractable* Interactable = Cast<AInteractable>(CurrentHit.GetActor());
			if (Interactable->IsA(AInteractable_Fuse::StaticClass())) {
				HasFuse = true;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Player has fuse")));
				Interactable->Interact();
			}
			else if (Interactable->IsA(AInteractable_Fuse_Holder::StaticClass())) {
				if (HasFuse) {
					HasFuse = false;
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Player has placed fuse")));
					Interactable->Interact();
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Player has no fuse")));
				}
			}
			else if (Interactable->IsA(AInteractable_Powered_Switch::StaticClass())) {
				AInteractable_Powered_Switch* Switch = Cast<AInteractable_Powered_Switch>(Interactable);
				if (Switch->FuseHolder->IsPowered) {
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Player has used powered switch")));
					Interactable->Interact();
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Fuse Holder is not powered")));
				}
			}
			else if (Interactable->IsA(AInteractable_Switch::StaticClass())) {
				Interactable->Interact();
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Player used %s"), *Interactable->Name));
			}
			
		}
	}
}

FString AMyProject6Character::MyRole()
{
	if (Role == ROLE_Authority)
	{
		return TEXT("SERVER");
	}
	else {
		return TEXT("CLIENT");
	}
}

