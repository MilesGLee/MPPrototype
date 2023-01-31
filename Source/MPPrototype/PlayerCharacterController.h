// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterController.generated.h"

UENUM()
enum RotationType 
{
	STATIC UMETA(DisplayName = "Static Rotation"),
	DYNAMIC UMETA(DisplayName = "Dynamic Rotation"),
	DYNAMICMOVEMENT UMETA(DisplayName = "Dynamic Movement Rotation"),
};

UCLASS()
class MPPROTOTYPE_API APlayerCharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Move
	UFUNCTION(BlueprintCallable)
	void ApplyMovement(float horizontal, float vertical, float speed);

	// Look turn
	UFUNCTION(BlueprintCallable)
	void ApplyLookRotation(float horizontal, float vertical, float speed);

	// Rotate mesh
	UFUNCTION(BlueprintCallable)
	void RotateMeshComponent();

private:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* _camera;

	// Spring Arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* _cameraArm;

	// Arm Socket
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	AActor* _armSocket;

	// Rotation type enumerator
	UPROPERTY(EditAnywhere, Category = Movement)
	TEnumAsByte<RotationType> _rotationType;

	// The player model
	UPROPERTY(EditAnywhere, Category = Movement)
	UMeshComponent* _playerMesh;
};
