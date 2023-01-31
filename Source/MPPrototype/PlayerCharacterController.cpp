// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"

// Sets default values
APlayerCharacterController::APlayerCharacterController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

