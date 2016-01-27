// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GravityManager.generated.h"

class USceneComponent;

UCLASS()
class FIREFLY_API AGravityManager: public AActor {
	GENERATED_BODY()

public:	
	/** Constructor. */
	AGravityManager();

	/** Destructor. */
	virtual ~AGravityManager();

	/** Ask for the actor to be orientated along gravity. */
	UFUNCTION(BlueprintCallable, Category = "GravityManager")
		static void RotateActorAlongGravityDirection(AActor* target);

	/** Ask for the component to be orientated along gravity. */
	UFUNCTION(BlueprintCallable, Category = "GravityManager")
		static void RotateComponentAlongGravityDirection(USceneComponent* target);
};
