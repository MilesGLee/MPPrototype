// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MPPROTOTYPE_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

	void setIsLockOn(bool isLockedOn) { m_isLockOn = isLockedOn; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Reference to the player's camera component
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Lock On")
	class UCameraComponent* PlayerCamera;

	// Reference to the actor that is currently locked on
	UPROPERTY(BlueprintReadOnly, Category = "Lock On")
		AActor* LockedOnTarget;

private:
	bool m_isLockOn = false;
};
