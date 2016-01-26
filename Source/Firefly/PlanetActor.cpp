// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "PlanetActor.h"
#include "FireflyPawn.h"

APlanetActor const* APlanetActor::m_planet = nullptr;

APlanetActor::APlanetActor() {
	// Create the scene component.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent0"));

	// Create the mesh component and its mesh.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent0"));
	MeshComponent->AttachParent = RootComponent;
	MeshComponent->SetCollisionProfileName("BlockAll");
	MeshComponent->SetStaticMesh(nullptr);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Initialization.
	PlanetMesh = nullptr;
	PlanetMeshScale = FVector(1.0f, 1.0f, 1.0f);
	GravityPower = 980.0f;
	bShouldUseStepping = true;

	// Call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

APlanetActor::~APlanetActor() {
}

void APlanetActor::PostInitializeComponents() {
	Super::PostInitializeComponents();
	Initialization();
}

#if WITH_EDITOR

void APlanetActor::PostEditChangeProperty(FPropertyChangedEvent& event) {
	Super::PostEditChangeProperty(event);
	Initialization();
}

#endif //WITH_EDITOR

void APlanetActor::Initialization() {
	// Set the mesh for the planet.
	if (PlanetMesh != nullptr) 	{
		MeshComponent->SetStaticMesh(PlanetMesh);
	}

	MeshComponent->SetRelativeScale3D(PlanetMeshScale);

	if (m_planet) {
		UE_LOG(LogTemp, Warning, TEXT("A PlanetActor has already been referenced."));
	}
	
	m_planet = this;
}

FVector APlanetActor::GetGravityDirection(const FVector& targetLocation) const {
	return (GetActorLocation() - targetLocation).GetSafeNormal();
}

void APlanetActor::SetGravityPower(float newGravity) {
	GravityPower = newGravity;
}

FVector APlanetActor::GetGravity(const FVector& targetLocation) {
	return m_planet ? m_planet->GetGravityDirection(targetLocation) : FVector(0.f, 0.f, -1.f);
}
