// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlanetActor.generated.h"

UCLASS()
class FIREFLY_API APlanetActor: public AActor {
	GENERATED_BODY()

public:
	/** Constructor. */
	APlanetActor();

	/** Destructor. */
	virtual ~APlanetActor();

	// AActor interface
	/** Call Initialization(). */
	virtual void PostInitializeComponents() override;

#if WITH_EDITOR
	/** Call Initialization(). */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& event) override;
#endif // WITH_EDITOR

	/** Link the planet to the pawn. */
	virtual void BeginPlay() override;
	// End of AActor interface

	/** Planet static mesh. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet Actor : General Settings")
		UStaticMesh* PlanetMesh;
	
	/** Planet mesh scale. */
	UPROPERTY(EditAnywhere, Category = "Planet Actor : General Settings")
		FVector PlanetMeshScale;

	/** Planet gravity power. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet Actor : General Settings")
		float GravityPower = 980.0f;

	/** If true, forces sub-stepping. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Planet Actor : General Settings")
		bool bShouldUseStepping = true;

	/** Change planet gravity power. */
	UFUNCTION(BlueprintCallable, Category = "PlanetActor")
		void SetGravityPower(float newGravity);

	/** Return a specific gravity direction based on the target location. */
	UFUNCTION(BlueprintCallable, Category = "PlanetActor")
		FVector GetGravityDirection(const FVector& targetLocation) const;

private:
	/** The root component of the planet. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		USceneComponent* PlanetRootComponent;

	/** Planet mesh component. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Ball, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;

protected:
	/** Initialize properties of the planet. */
	virtual void Initialization();

public:
	/** Return Ball subobject. **/
	FORCEINLINE class UStaticMeshComponent* GetPlanetMesh() const { return MeshComponent; }

	/** Return PlanetRootComponent subobject. **/
	FORCEINLINE class USceneComponent* GetPlanetRootComponent() const { return PlanetRootComponent; }
};

