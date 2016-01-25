// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DirectedLightActor.generated.h"

class UDirectionalLightComponent;

/**
 * 
 */
UCLASS()
class FIREFLY_API ADirectedLightActor : public AActor {
	GENERATED_BODY()

	/** The directional light. */
	UPROPERTY(Category = Direction, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UDirectionalLightComponent* m_light;

public:
	/** Constructor. */
	ADirectedLightActor();

	/** Destructor. */
	virtual ~ADirectedLightActor();

	/** The actor to follow. */
	UPROPERTY(Category = Direction, BlueprintReadWrite, EditAnywhere)
		AActor const* Target;
	
	/** The actor that is considered as the center of the world. */
	UPROPERTY(Category = Direction, BlueprintReadWrite, EditAnywhere)
		AActor const* Center;

	// Begin AActor override.
	/** Recalculate the light direction. */
	virtual void Tick(float deltaSeconds) override;
	// End AActor override.
};
