// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimationController.h"
#include "PlayerCharacterController.h"
#include <Components/SkeletalMeshComponent.h>

// Sets default values for this component's properties
UPlayerAnimationController::UPlayerAnimationController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAnimationController::BeginPlay()
{
	Super::BeginPlay();

	// ...

	_characterSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}


// Called every frame
void UPlayerAnimationController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (_currentAnimationState == AnimationState::IDLE) 
	{
		if (_characterIdleAnimation)
		{
			_characterSkeletalMesh->PlayAnimation(_characterIdleAnimation, true);
		}
	}
	else if (_currentAnimationState == AnimationState::MOVING)
	{
		if (_characterRunAnimation)
		{
			_characterSkeletalMesh->PlayAnimation(_characterRunAnimation, true);
		}
	}
	else if (_currentAnimationState == AnimationState::ATTACKING)
	{
		if (_characterAttackAnimation)
		{
			_characterSkeletalMesh->PlayAnimation(_characterAttackAnimation, true);
		}
	}
}

void UPlayerAnimationController::SetState(TEnumAsByte<AnimationState> state)
{
	_currentAnimationState = state;
}

