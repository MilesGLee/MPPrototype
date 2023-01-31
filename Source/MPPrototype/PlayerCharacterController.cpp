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
	_cameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	_cameraArm->SetupAttachment(RootComponent);
	_cameraArm->TargetArmLength = 300.0f;
	_cameraArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	_cameraArm->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Create Camera
	_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	_camera->SetupAttachment(_cameraArm);

	// Get Mesh
	_playerMesh = GetMesh();
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

	if (_rotationType == RotationType::DYNAMIC)
	{
		RotateMeshComponent();
	}
}

// Called to bind functionality to input
void APlayerCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacterController::ApplyMovement(float horizontal, float vertical, float speed)
{
	FVector forward = _camera->GetForwardVector();
	FVector right = _camera->GetRightVector();
	FVector direction;

	direction = forward * vertical;
	direction += right * horizontal;

	direction.Normalize();
	AddMovementInput(direction, speed);

	if (_rotationType == RotationType::DYNAMICMOVEMENT) 
	{
		if (!direction.IsZero()) 
		{
			RotateMeshComponent();
		}
	}
}

void APlayerCharacterController::ApplyLookRotation(float horizontal, float vertical, float speed)
{
	FRotator rotation(0.0f, 0.0f, 0.0f);

	rotation.Add(vertical, horizontal, 0.0f);

	_cameraArm->AddRelativeRotation(rotation);
}

void APlayerCharacterController::RotateMeshComponent()
{
	FRotator rotation(0.0f, 0.0f, 0.0f);

	FRotator desiredRotation(0.0f, -90.0f + _cameraArm->GetRelativeRotation().Yaw, 0.0f);

	FQuat desiredQuat = desiredRotation.Quaternion();

	rotation = FQuat::Slerp(_playerMesh->GetRelativeRotation().Quaternion(), desiredQuat, 0.05f).Rotator();

	_playerMesh->SetRelativeRotation(rotation);
}

