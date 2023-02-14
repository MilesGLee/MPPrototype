// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "PlayerCharacterController.h"
#include <PhysicsEngine/RigidBodyBase.h>
#include <Engine/World.h>


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	m_physics = Cast<UPrimitiveComponent>(this);
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MovePlayerBelowMap();
}

void UAbilityComponent::MovePlayerBelowMap()
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	if (m_willSink && m_sinks < 1)
	{

		CurrentLocation -= m_sinkSpot;
		GetOwner()->SetActorRelativeLocation(CurrentLocation);
		GetOwner()->DisableComponentsSimulatePhysics();
		Cast<ACharacter>(GetOwner())->GetMovementBase()->SetEnableGravity(false);
		m_sinks++;
	}
}

void UAbilityComponent::MovePlayerBackUp()
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	if (!m_willSink && m_sinks > 0 )
	{
		m_physics->SetSimulatePhysics(true);

		CurrentLocation += m_sinkSpot;
		GetOwner()->SetActorRelativeLocation(CurrentLocation);
		Cast<ACharacter>(GetOwner())->GetMovementBase()->SetEnableGravity(true);
		m_sinks--;
	}
}

