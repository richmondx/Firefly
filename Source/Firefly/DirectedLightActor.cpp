// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "DirectedLightActor.h"

ADirectedLightActor::ADirectedLightActor() {
	// Creation of the light component.
	m_light = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("LightComponent0"));
	RootComponent = m_light;

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
		SetActorRotation(FQuat::FindBetween(FVector(0.8f, 0.f, 0.2f), Center->GetActorLocation() - Target->GetActorLocation()));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("DirectedLight Center or Target has not been set. Tick function will be disabled."));
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}
