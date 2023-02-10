// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAnimationController.generated.h"

// The enum class for the different types of rotations the mesh will have while in play
UENUM()
enum AnimationState
{
	IDLE UMETA(DisplayName = "Idle"),
	MOVING UMETA(DisplayName = "Moving"),
	ATTACKING UMETA(DisplayName = "Attacking"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MPPROTOTYPE_API UPlayerAnimationController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerAnimationController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TEnumAsByte<AnimationState> GetState() { return _currentAnimationState; }

	void SetState(TEnumAsByte<AnimationState> state);

private:
	// Which way the player mesh rotates
	UPROPERTY(EditAnywhere, Category = Anims)
	TEnumAsByte<AnimationState> _currentAnimationState;

	UPROPERTY(EditAnywhere, Category = Anims)
	class USkeletalMeshComponent* _characterSkeletalMesh;

	UPROPERTY(EditAnywhere, Category = Anims)
	class UAnimationAsset* _characterIdleAnimation;

	UPROPERTY(EditAnywhere, Category = Anims)
	class UAnimationAsset* _characterRunAnimation;

	UPROPERTY(EditAnywhere, Category = Anims)
	class UAnimationAsset* _characterAttackAnimation;
};
