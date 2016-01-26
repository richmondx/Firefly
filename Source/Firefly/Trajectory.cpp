// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "Trajectory.h"
#include "Components/SplineComponent.h"

ATrajectory::ATrajectory() {
	m_spline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent0"));
	RootComponent = m_spline;

	PrimaryActorTick.bCanEverTick = false;
}

ATrajectory::~ATrajectory() {
}

void ATrajectory::Follow(AActor* actor, float time) const {
	FVector position = m_spline->GetLocationAtTime(FMath::Fmod(time, m_spline->Duration), ESplineCoordinateSpace::World, true);
	FVector orientation = m_spline->GetDirectionAtTime(FMath::Fmod(time, m_spline->Duration), ESplineCoordinateSpace::World, true);
	
	FQuat delta = FQuat::FindBetween(actor->GetActorForwardVector(), orientation);
	FQuat rotation = delta * actor->GetActorRotation().Quaternion();
	
	actor->SetActorLocationAndRotation(position, rotation);
}