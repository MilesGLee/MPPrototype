// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include <GameFramework/PlayerController.h>
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
	
}


// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MovePlayerBelowMap();
}

void UAbilityComponent::MovePlayerBelowMap()
{
	FVector CurrentLocation = GetOwner()->GetVelocity();

	if (m_willSink && m_sinks < 1)
	{

		CurrentLocation -= m_sinkSpot;
		GetOwner()->SetActorRelativeLocation(CurrentLocation);
		m_sinks++;
	}
}

void UAbilityComponent::MovePlayerBackUp()
{
	FVector CurrentLocation = GetOwner()->GetVelocity();

	if (!m_willSink && m_sinks > 0 )
	{
		CurrentLocation += m_sinkSpot;
		GetOwner()->SetActorRelativeLocation(CurrentLocation);
		m_sinks--;
	}
}

