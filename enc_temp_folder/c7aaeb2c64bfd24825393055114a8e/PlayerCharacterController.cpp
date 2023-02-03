// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"
#include "DrawDebugHelpers.h"
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
	
	_dashAvailable = true;
}

// Called every frame
void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_rotationType == RotationType::DYNAMIC)
	{
		RotateMeshComponent(_rotateSpeed);
	}

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
	if (_currentAct != ActingType::NOTACTING)
		return;

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
			RotateMeshComponent(_rotateSpeed);
		}
	}
}

void APlayerCharacterController::ApplyLookRotation(float horizontal, float vertical, float speed)
{
	FRotator rotation(0.0f, 0.0f, 0.0f);

	rotation.Add(vertical, horizontal, 0.0f);

	_cameraArm->AddRelativeRotation(rotation);
}

void APlayerCharacterController::RotateMeshComponent(float speed)
{
	if (_currentAct != ActingType::NOTACTING)
		return;

	FRotator rotation(0.0f, 0.0f, 0.0f);

	FRotator desiredRotation(0.0f, -90.0f + _cameraArm->GetRelativeRotation().Yaw, 0.0f);

	FQuat desiredQuat = desiredRotation.Quaternion();

	rotation = FQuat::Slerp(_playerMesh->GetRelativeRotation().Quaternion(), desiredQuat, speed).Rotator();

	_playerMesh->SetRelativeRotation(rotation);
}

void APlayerCharacterController::StartDash(float horizontal, float vertical, float distance)
{
	if (!_dashAvailable)
		return;

	FVector forward = _camera->GetForwardVector();
	FVector right = _camera->GetRightVector();
	FVector direction;

	direction = forward * vertical;
	direction += right * horizontal;

	if (horizontal == 0 && vertical == 0)
	{
		direction = forward;
	}

	direction.Z = 0.0f;

	direction.Normalize();

	FVector startPosition = GetActorLocation();

	FVector endPosition = startPosition + direction * distance;

	FHitResult hit;
	FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
	FCollisionResponseParams responseParams;
	queryParams.AddIgnoredComponent(GetMesh());
	GetWorld()->LineTraceSingleByChannel(hit, startPosition, endPosition, ECollisionChannel::ECC_Visibility, queryParams, responseParams);
	if (hit.Location != FVector::ZeroVector) 
	{
		_dashDestination = hit.Location;
	}
	else 
	{
		_dashDestination = endPosition;
	}

	GetMovementComponent()->Velocity = FVector(0.0f, 0.0f, 0.0f);

	//FRotator desiredRotation(0.0f, -90.0f + direction.Y, 0.0f);
	//_playerMesh->SetRelativeRotation(desiredRotation);

	_dashAvailable = false;
	_currentAct = ActingType::ACTINGDYNAMIC;
	_isDashing = true;
	//GetWorldTimerManager().SetTimer(_dashTimerHandle, this, &APlayerCharacterController::Dash, 0.01f, 1.0f);
}

void APlayerCharacterController::Dash()
{
	if (FVector::Dist(_dashDestination, GetActorLocation()) <= 50.0f)
	{
		GetWorldTimerManager().ClearTimer(_dashTimerHandle);
		_isDashing = false;
		EndDash();

		return;
	}

	GetCharacterMovement()->GravityScale = 0;
	GetMovementComponent()->Velocity = FVector(0.0f, 0.0f, 0.0f);
	SetActorRelativeLocation(FMath::Lerp(GetActorLocation(), _dashDestination, _dashSpeed));
}

void APlayerCharacterController::EndDash()
{
	GetCharacterMovement()->GravityScale = 1;
	_currentAct = ActingType::NOTACTING;
	FTimerHandle MemberTimerHandle;
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &APlayerCharacterController::ResetDash, 1.0f, 1.0f);
}

void APlayerCharacterController::ResetDash()
{
	_dashAvailable = true;
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

