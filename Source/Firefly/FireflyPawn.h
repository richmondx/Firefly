// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/Pawn.h"
#include "FireflyPawn.generated.h"

class APlanetActor;
class UFireflyMovementComponent;

UCLASS(config = Game)
class FIREFLY_API AFireflyPawn : public APawn {
	GENERATED_BODY()

	/** The capsule component. */
	UPROPERTY(Category = Movement, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_capsule;

	/** The movement component of the pawn. */
	UPROPERTY(Category = Movement, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UFireflyMovementComponent* m_movement;

	/** StaticMesh component that will be the visuals for our flying pawn. */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_mesh;

	/** Spring arm that will offset the camera. */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_springArm;

	/** Camera component that will be our viewpoint. */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_camera;

public:
	/** Constructor. */
	AFireflyPawn();

	/** Destructor. */
	virtual ~AFireflyPawn();

	/** Set the planet for this character. */
	void SetPlanet(APlanetActor const *planet);

	// Begin AActor overrides.
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* myComp, class AActor* other, class UPrimitiveComponent* otherComp, bool bSelfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit);
	// End AActor overrides.

protected:
	// Begin APawn overrides.
	/** Allows binding actions/axes to functions. */
	virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;
	// End APawn overrides.
																							
	/** As long as requested, increase the speed of the pawn. */
	void StartSpeedUp();
	void StopSpeedUp();

	/** Bound to the vertical axis */
	void MoveUpInput(float value);

	/** Bound to the horizontal axis */
	void MoveRightInput(float value);

public:
	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return m_mesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return m_springArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return m_camera; }
};