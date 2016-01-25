// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "CollisionSphere.generated.h"

/**
 * 
 */
UCLASS()
class FIREFLY_API UCollisionSphere : public USphereComponent {
	GENERATED_BODY()
	
public:
	/** Constructor. */
	UCollisionSphere();

	/** Destructor. */
	virtual ~UCollisionSphere();

#if 0
	// Begin PrimitiveComponent override.
	virtual bool MoveComponentImpl(const FVector& delta, const FQuat& newRotationQuat, bool bSweep, FHitResult* outHit, EMoveComponentFlags moveFlags, ETeleportType teleport) override;
	// End PrimitiveComponent override
#endif
};
