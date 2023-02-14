// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MPPROTOTYPE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(CallInEditor)
	void MovePlayerBelowMap();

	UFUNCTION(CallInEditor)
	void MovePlayerBackUp();


private:
	UPROPERTY(EditAnywhere)
	bool m_willSink;

	UPROPERTY(EditAnywhere)
	FVector m_sinkSpot;

	class APlayerController* m_controller;

	INT32 m_sinks;

	class UPrimitiveComponent* m_physics;
};
