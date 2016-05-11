// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/FloatingPawnMovement.h"
#include "FireflyMovementComponent.generated.h"

class APlanetActor;

UCLASS()
class FIREFLY_API UFireflyMovementComponent : public UFloatingPawnMovement {
	GENERATED_BODY()

	/** The capsule component of the pawn. */
	USphereComponent* m_sphere;

public:
	/** Constructor. */
	UFireflyMovementComponent();

	/** Destructor. */
	virtual ~UFireflyMovementComponent();

	/** Set the owner of this movement component. */
	void SetMovementOwner(APawn *owner);

	// Begin UActorComponent Interface
	virtual void InitializeComponent() override;
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) override;
	// End UActorComponent Interface

	/** Information about the surface the pawn collided with. */
	UPROPERTY(Category = "Firefly Movement Component", VisibleInstanceOnly, BlueprintReadOnly)
		FHitResult CapsuleHitResult;

	/** How quickly the firefly can steer */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float TurnSpeed;

	/** Acceleration minimal value. */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float MinAcceleration;

	/** Acceleration maximal value. */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float MaxAcceleration;

	/** Upper bound for max speed. */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float MMaxSpeed;

	/** The max speed variation when boosting or braking. */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float Variation;

	/** Lower bound for max speed. */
	UPROPERTY(Category = "Firefly Movement Component", EditAnywhere)
		float MMinSpeed;

	/** Upgrade or decrease the speed of the firefly. */
	void SpeedUp(bool bSpeedUp);

	/** Boost the firefly. */
	void Boost();

	/** Brake the firefly. */
	void Brake();

	/** Move the firefly up or down. */
	void MoveUp(float value);

	/** Move the firefly left or right. */
	void MoveRight(float value);

	/** Change the orientation of the pawn. */
	void Reorientate(FRotator const &delta);

private:
	/** State whether the pawn should increase its speed. */
	bool m_bSpeeding;

	/** Current forward speed. */
	float m_currentSpeed;

	/** Current yaw speed. */
	float m_currentYawSpeed;

	/** Current pitch speed. */
	float m_currentPitchSpeed;

	/** Current roll speed. */
	float m_currentRollSpeed;

	/** HMD roll value. */
	float m_rollHMD;

	/** The orientation of the firefly, relatively to the ground. */
	FRotator m_orientation;
};
