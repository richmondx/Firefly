// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "DirectedLightActor.h"

ADirectedLightActor::ADirectedLightActor() {
	// Creation of the light component.
	RootComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLightComponent0"));

	Target = nullptr;
	Center = nullptr;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

ADirectedLightActor::~ADirectedLightActor() {
}

void ADirectedLightActor::Tick(float deltaSeconds) {
	Super::Tick(deltaSeconds);

	if (Center && Target) {
		SetActorRotation(FQuat::FindBetween(FVector(1.f, 0.f, 0.f), Center->GetActorLocation() - Target->GetActorLocation()));
	}
}
