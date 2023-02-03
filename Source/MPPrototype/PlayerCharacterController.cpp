// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "TimerManager.h"
#include <GameFramework/CharacterMovementComponent.h>
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
	
	// Allow the player to dash at the beginning of play
	_dashAvailable = true;
}

// Called every frame
void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the rotation type is dynamic, rotate the mesh at all times
	if (_rotationType == RotationType::DYNAMIC)
	{
		RotateMeshComponent(_rotateSpeed);
	}

	// If the player is dashing, call dash function
	if(_isDashing)
		Dash();
}

// Called to bind functionality to input
void APlayerCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacterController::ApplyMovement(float horizontal, float vertical, float speed)
{
	// If the player is not acting
	if (_currentAct != ActingType::NOTACTING)
		return;

	// get the cameras forward, and right
	FVector forward = _camera->GetForwardVector();
	FVector right = _camera->GetRightVector();
	FVector direction;

	direction = forward * vertical;
	direction += right * horizontal;

	direction.Normalize();

	// Move the player
	AddMovementInput(direction, speed);

	// If the rotation type is dynamic movement, only rotate the mesh if the player is moving
	if (_rotationType == RotationType::DYNAMICMOVEMENT) 
	{
		if (!direction.IsZero()) 
		{
			RotateMeshComponent(_rotateSpeed);
		}
	}
}

// Rotate the camera based on mouse input
void APlayerCharacterController::ApplyLookRotation(float horizontal, float vertical, float speed)
{
	FRotator rotation(0.0f, 0.0f, 0.0f);

	rotation.Add(vertical, horizontal, 0.0f);

	_cameraArm->AddRelativeRotation(rotation);
}

void APlayerCharacterController::RotateMeshComponent(float speed)
{
	// If the player is not acting
	if (_currentAct != ActingType::NOTACTING)
		return;

	FRotator rotation(0.0f, 0.0f, 0.0f);

	// Get mesh direction with meshes origin direction applied
	FRotator desiredRotation(0.0f, -90.0f + _cameraArm->GetRelativeRotation().Yaw, 0.0f);

	FQuat desiredQuat = desiredRotation.Quaternion();

	// Slerp the players relative rotation to the desired rotation
	rotation = FQuat::Slerp(_playerMesh->GetRelativeRotation().Quaternion(), desiredQuat, speed).Rotator();

	// Rotate the mesh with the slerped rotation
	_playerMesh->SetRelativeRotation(rotation);
}

void APlayerCharacterController::StartDash(float horizontal, float vertical, float distance)
{
	// If the player can dash
	if (!_dashAvailable)
		return;

	// Get the direction from the camera and input
	FVector forward = _camera->GetForwardVector();
	FVector right = _camera->GetRightVector();
	FVector direction;

	direction = forward * vertical;
	direction += right * horizontal;

	// If the player is not inputting a direction, just go forward
	if (horizontal == 0 && vertical == 0)
	{
		direction = forward;
	}
	
	// Make sure the player can't move up or down while dashing
	direction.Z = 0.0f;

	direction.Normalize();

	FVector startPosition = GetActorLocation();
	FVector endPosition = startPosition + direction * distance;

	// Cast a ray in the direction we want to dash
	FHitResult hit;
	FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
	FCollisionResponseParams responseParams;
	queryParams.AddIgnoredComponent(GetMesh());
	GetWorld()->LineTraceSingleByChannel(hit, startPosition, endPosition, ECollisionChannel::ECC_Visibility, queryParams, responseParams);
	// If the ray hit a wall or reached its end, set the dash end location to the point
	if (hit.Location != FVector::ZeroVector) 
	{
		_dashDestination = hit.Location;
	}
	else 
	{
		_dashDestination = endPosition;
	}

	// Make the player have no velocity while dashing so they can't fall
	GetMovementComponent()->Velocity = FVector(0.0f, 0.0f, 0.0f);

	// Rotate the players mesh to match the dashed direction
	FVector desiredDirection = endPosition - startPosition;
	FRotator desiredRotation = desiredDirection.Rotation();
	desiredRotation.Yaw += -90.0f;

	FRotator rotation = FQuat::Slerp(_playerMesh->GetRelativeRotation().Quaternion(), desiredRotation.Quaternion(), 1.0f).Rotator();

	_playerMesh->SetRelativeRotation(rotation);

	// Set the players state, if dashing is available, and that the player is dashing
	_dashAvailable = false;
	_currentAct = ActingType::ACTINGDYNAMIC;
	_isDashing = true;
}

void APlayerCharacterController::Dash()
{
	// If the player has reached the end dash location
	if (FVector::Dist(_dashDestination, GetActorLocation()) <= 50.0f)
	{
		// Stop dashing
		_isDashing = false;
		EndDash();

		return;
	}

	// Make the player have no velocity and gravity while dashing so they can't fall
	GetCharacterMovement()->GravityScale = 0;
	GetMovementComponent()->Velocity = FVector(0.0f, 0.0f, 0.0f);

	// Lerp the players position over time to the dash destination
	SetActorRelativeLocation(FMath::Lerp(GetActorLocation(), _dashDestination, _dashSpeed));
}

void APlayerCharacterController::EndDash()
{
	// Return gravity and set players state
	GetCharacterMovement()->GravityScale = 1;
	_currentAct = ActingType::NOTACTING;

	// Begin the dash cooldown
	GetWorldTimerManager().SetTimer(_dashCooldownHandle, this, &APlayerCharacterController::ResetDash, 1.0f, 1.0f);
}

// Allow the player to dash after the cooldown has passed
void APlayerCharacterController::ResetDash()
{
	_dashAvailable = true;
	GetWorldTimerManager().ClearTimer(_dashCooldownHandle);
}

