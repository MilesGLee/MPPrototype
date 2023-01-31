// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

// Sets default values
APlayerCharacterController::APlayerCharacterController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Arm
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 300.0f;
	CameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	CameraArm->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Create Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(CameraArm);
}

// Called when the game starts or when spawned
void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacterController::ApplyMovement(float horizontal, float vertical, float speed)
{
	FVector forward = Camera->GetForwardVector();
	FVector right = Camera->GetRightVector();
	FVector direction;

	direction = forward * vertical;
	direction += right * horizontal;

	direction.Normalize();
	AddMovementInput(direction, speed);
}

void APlayerCharacterController::ApplyLookRotation(float horizontal, float vertical, float speed)
{

}

