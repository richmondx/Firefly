// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Trajectory.generated.h"

class USplineComponent;

UCLASS()
class FIREFLY_API ATrajectory : public AActor {
	GENERATED_BODY()

	/** The spline component of this trajectory. */
	UPROPERTY(Category = Direction, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USplineComponent* m_spline;
	
public:	
	/** Constructor. */
	ATrajectory();

	/** Destructor. */
	virtual ~ATrajectory();

	/** Move and rotate the object in order to match the trajectory. */
	UFUNCTION(BlueprintCallable, Category = "Trajectory")
		void Follow(AActor* actor, float time) const;
};
