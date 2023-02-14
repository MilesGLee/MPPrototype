// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterController.generated.h"

// The enum class for the different types of rotations the mesh will have while in play
UENUM()
enum RotationType 
{
	STATIC UMETA(DisplayName = "No Rotation"),
	DYNAMIC UMETA(DisplayName = "Constant Rotation"),
	DYNAMICMOVEMENT UMETA(DisplayName = "Movement Rotation"),
};

// The states of the player character to decide when the mesh can move and or rotate
UENUM()
enum ActingType
{
	NOTACTING UMETA(DisplayName = "Free Movement"),
	ACTINGDYNAMIC UMETA(DisplayName = "Locked Rotation"),
	ACTINGROOTED UMETA(DisplayName = "Locked Movement"),
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

	// Calculate direction and move the player in the inputted direction
	UFUNCTION(BlueprintCallable)
	void ApplyMovement(float horizontal, float vertical, float speed);

	// Rotate the camera component based on the mouse input
	UFUNCTION(BlueprintCallable)
	void ApplyLookRotation(float horizontal, float vertical, float speed);

	// Rotate the mesh based on the direction its moving in and the rotation type
	UFUNCTION(BlueprintCallable)
	void RotateMeshComponent(float speed);

	// Swiftly moves the character over a distance in a straight line based on inputted direction
	UFUNCTION(BlueprintCallable)
	void StartDash(float horizontal, float vertical, float distance);

	// Dashes once the direction and distance have been calculated in the start dash function
	void Dash();

	// Ends the dash and changes the state of the player
	void EndDash();

	// Allows the player to dash again once the cooldown has finished
	void ResetDash();

private:
	// The camera component of the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* _camera;

	// The spring arm component of the player, used to move the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* _cameraArm;

	// Which way the player mesh rotates
	UPROPERTY(EditAnywhere, Category = Movement)
	TEnumAsByte<RotationType> _rotationType;

	// How fast the mesh rotates
	UPROPERTY(EditAnywhere, Category = Movement)
	float _rotateSpeed = 0.05f;

	// The player model
	UPROPERTY(EditAnywhere, Category = Movement)
	UMeshComponent* _playerMesh;

	// The timer handle that handles the dashes cooldown
	FTimerHandle _dashCooldownHandle;

	// The end point where the player will dash to
	FVector _dashDestination;

	// The dash speed
	UPROPERTY(EditAnywhere, Category = MovementDash)
	float _dashSpeed;

	// The dash cooldown
	UPROPERTY(EditAnywhere, Category = MovementDash)
	float _dashCooldown;

	// Booleans to handle when the player can dash and if the player is dashing
	bool _dashAvailable, _isDashing;

	// The current state of the player
	TEnumAsByte<ActingType> _currentAct;

	UPROPERTY(EditAnywhere, Category = Movement)
	FVector _currentOrientation;
};
