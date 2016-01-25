// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyMovementComponent.h"
#include "FireflyPawn.h"
#include "PlanetActor.h"

UFireflyMovementComponent::UFireflyMovementComponent() {
	// Set actors and components.
	m_planet = nullptr;
	m_sphere = nullptr;

	// Set handling parameters.
	MinAcceleration = 300.f;
	MaxAcceleration = 7000.f;
	Acceleration = MinAcceleration;
	Deceleration = 500.f;
	MaxSpeed = 700.f;
	TurnSpeed = 20.f;
	m_bSpeeding = false;
	m_currentForwardSpeed = 0.f;
	m_currentYawSpeed = 0.f;
	m_currentPitchSpeed = 0.f;
	m_currentRollSpeed = 0.f;
	m_orientation = FRotator::ZeroRotator;

	// Call Tick() every frame.
	PrimaryComponentTick.bCanEverTick = true;
}

UFireflyMovementComponent::~UFireflyMovementComponent() {
}

void UFireflyMovementComponent::SetMovementOwner(APawn* owner) {
	PawnOwner = owner;
}

void UFireflyMovementComponent::InitializeComponent() {
	Super::InitializeComponent();
	m_sphere = Cast<USphereComponent>(UpdatedComponent);
}

void UFireflyMovementComponent::SetPlanet(APlanetActor const* planet) {
	m_planet = planet;
}

void UFireflyMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	// Move plan forwards (with sweep so we stop when we collide with things).
	m_sphere->AddLocalOffset(FVector(m_currentForwardSpeed * deltaTime, 0.f, 0.f), true);

	// Calculate change in rotation this frame.
	FVector invGravity = m_planet ? -m_planet->GetGravityDirection(m_sphere->GetComponentLocation()) : FVector(0.f, 0.f, 1.f);
	const FQuat deltaQuat = FQuat::FindBetween(m_sphere->GetUpVector(), invGravity);
	const FQuat targetQuat = deltaQuat * m_sphere->GetComponentRotation().Quaternion();
	m_sphere->SetWorldRotation(targetQuat);

	m_orientation.Pitch += m_currentPitchSpeed * deltaTime;
	m_orientation.Yaw = m_currentYawSpeed * deltaTime;
	//m_orientation.Roll = m_currentRollSpeed * deltaTime;
	m_sphere->AddLocalRotation(m_orientation);

	// Calculate new acceleration and speed.
	float currentAcc;
	if (m_bSpeeding) {
		currentAcc = Acceleration;
		Acceleration += 500.f * GetWorld()->GetDeltaSeconds();
	} else {
		currentAcc = -Deceleration;
		Acceleration -= 5000.f * GetWorld()->GetDeltaSeconds();
	}

	Acceleration = FMath::Clamp(Acceleration, MinAcceleration, MaxAcceleration);
	float newForwardSpeed = m_currentForwardSpeed + (GetWorld()->GetDeltaSeconds() * currentAcc);
	m_currentForwardSpeed = FMath::Clamp(newForwardSpeed, 0.f, MaxSpeed);
}

void UFireflyMovementComponent::SpeedUp(bool bSpeedUp) {
	m_bSpeeding = bSpeedUp;
}

void UFireflyMovementComponent::MoveUp(float value) {
	// Target pitch speed is based in input
	float TargetPitchSpeed = (value * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(m_currentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	m_currentPitchSpeed = FMath::FInterpTo(m_currentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void UFireflyMovementComponent::MoveRight(float value) {
	// Target yaw speed is based on input
	float TargetYawSpeed = (value * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	m_currentYawSpeed = FMath::FInterpTo(m_currentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(value) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value
	float TargetRollSpeed = bIsTurning ? (m_currentYawSpeed * 0.2f) : (m_orientation.Roll * -2.f);

	// Smoothly interpolate roll speed
	m_currentRollSpeed = FMath::FInterpTo(m_currentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}