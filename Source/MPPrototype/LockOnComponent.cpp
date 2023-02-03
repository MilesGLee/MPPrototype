// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	// ...
	
}


// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerCamera)
		return;
	

	// Get the component's location and rotation
	FVector cameraLocation = PlayerCamera->GetComponentLocation();
	FRotator cameraRotation = PlayerCamera->GetComponentRotation();

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;


	// Calculate the end point of the raycast
	FVector EndPoint = cameraLocation + cameraRotation.Vector() * 10000.0f;

	// Perform the raycast
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, cameraLocation, EndPoint, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		// If the raycast hit an actor, store a reference to the actor in LockedOnTarget
		LockedOnTarget = HitResult.GetActor();

		FVector HitLocation = HitResult.Location;
		FVector ForwardVector = HitLocation - GetOwner()->GetActorLocation();

		FRotator LookAtRotation = ForwardVector.Rotation();
		GetOwner()->SetActorRotation(LookAtRotation);

		// Draw a debug line to visualize the raycast
		DrawDebugLine(GetWorld(), cameraLocation, HitResult.Location, FColor::Red, false, -1.0f, 0, 2.0f);
	}
	else
	{
		// If the raycast did not hit an actor, set LockedOnTarget to nullptr
		LockedOnTarget = nullptr;

	}
}

