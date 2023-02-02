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

UENUM()
enum ActingType
{
	NOTACTING UMETA(DisplayName = "Not Acting"),
	ACTINGDYNAMIC UMETA(DisplayName = "Dynamic Acting"),
	ACTINGROOTED UMETA(DisplayName = "Rooted Acting"),
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
	void RotateMeshComponent(float speed);

	// Dash character
	UFUNCTION(BlueprintCallable)
	void StartDash(float horizontal, float vertical, float distance);

	void Dash();

	void EndDash();

	void ResetDash();

private:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* _camera;

	// Spring Arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* _cameraArm;

	// Rotation type enumerator
	UPROPERTY(EditAnywhere, Category = Movement)
	TEnumAsByte<RotationType> _rotationType;

	// How fast the mesh rotates
	UPROPERTY(EditAnywhere, Category = Movement)
	float _rotateSpeed = 0.05f;

	// The player model
	UPROPERTY(EditAnywhere, Category = Movement)
	UMeshComponent* _playerMesh;

	// The dash timer handle
	FTimerHandle _dashTimerHandle;

	// The dash destination
	FVector _dashDestination;

	// The dash speed
	UPROPERTY(EditAnywhere, Category = MovementDash)
	float _dashSpeed;

	// The dash cooldown
	UPROPERTY(EditAnywhere, Category = MovementDash)
	float _dashCooldown;

	// The dash toggle
	bool _dashAvailable, _isDashing;

	// player acting bool
	TEnumAsByte<ActingType> _currentAct;
};
