// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/FloatingPawnMovement.h"
#include "FireflyMovementComponent.generated.h"

class APlanetActor;

UCLASS()
class FIREFLY_API UFireflyMovementComponent : public UFloatingPawnMovement {
	GENERATED_BODY()
	
	/** The planet actor. */
	APlanetActor const *m_planet;

	/** The capsule component of the pawn. */
	UCapsuleComponent* m_capsule;

public:
	/** Constructor. */
	UFireflyMovementComponent();

	/** Destructor. */
	virtual ~UFireflyMovementComponent();

	/** Set the owner of this movement component. */
	void SetMovementOwner(APawn *owner);

	/** Set the planet actor. */
	void SetPlanet(APlanetActor const *planet);

	// Begin UActorComponent Interface
	virtual void InitializeComponent() override;
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) override;
	// End UActorComponent Interface
	
	/** Called when the collision capsule touches another primitive component. */
	//void NotifyCapsuleHit(UPrimitiveComponent* myComp, AActor* other, UPrimitiveComponent* otherComp, bool bSelfMoved, 
	//		FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit);

	/** Update the orientation of the firefly to fit the gravity direction. */
	//void ApplyGravity(const FVector& force, bool bAllowSubstepping, bool bAccelChange);

	/** Information about the surface the pawn collided with. */
	UPROPERTY(Category = "Firefly Movement Component", VisibleInstanceOnly, BlueprintReadOnly)
		FHitResult CapsuleHitResult;

	/** How quickly the firefly can steer */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float TurnSpeed;

	/** Upgrade or decrease the speed of the firefly. */
	void SpeedUp(bool bSpeedUp);

	/** Move the firefly up or down. */
	void MoveUp(float value);

	/** Move the firefly left or right. */
	void MoveRight(float value);

private:
	/** State whether the pawn should increase its speed. */
	bool m_bSpeeding;

	/** Current forward speed. */
	float m_currentForwardSpeed;

	/** Current yaw speed. */
	float m_currentYawSpeed;

	/** Current pitch speed. */
	float m_currentPitchSpeed;

	/** Current roll speed. */
	float m_currentRollSpeed;

	/** The last orientation of the firefly, relatively to the ground. */
	FQuat m_lastOrientation;

	/** The last gravity normal. */
	FRotator m_lastNormal;
};
