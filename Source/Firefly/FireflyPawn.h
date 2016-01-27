// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/Pawn.h"
#include "FireflyPawn.generated.h"

class APlanetActor;
class UFireflyMovementComponent;

UCLASS(config = Game)
class FIREFLY_API AFireflyPawn : public APawn {
	GENERATED_BODY()

	/** The sphere component. */
	UPROPERTY(Category = Movement, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* m_sphere;

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

	/** The point of light emitted by the firefly. */
	UPROPERTY(Category = Light, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* m_light;

public:
	/** Constructor. */
	AFireflyPawn();

	/** Destructor. */
	virtual ~AFireflyPawn();

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
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveUpInput(float value);

	/** Bound to the horizontal axis */
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveRightInput(float value);

	/** Match the orientation of the camera with the one of the HMD. */
	UFUNCTION(BlueprintCallable, Category = "HMDOrientation")
		void OrientateCameraAlongHMD();

public:
	/** Returns FireflyMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return m_mesh; }
	/** Returns SpringArm subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return m_springArm; }
	/** Returns Camera subobject **/
	FORCEINLINE class UCameraComponent* GetCamera() const { return m_camera; }
};
