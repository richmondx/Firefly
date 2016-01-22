// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyMovementComponent.h"
#include "FireflyPawn.h"
#include "PlanetActor.h"

UFireflyMovementComponent::UFireflyMovementComponent() {
	// Set actors and components.
	m_planet = nullptr;
	m_capsule = nullptr;

	// Set handling parameters.
	Acceleration = 7000.f;
	Deceleration = 3000.f;
	MaxSpeed = 700.f;
	TurnSpeed = 40.f;
	m_bSpeeding = false;
	m_currentForwardSpeed = 0.f;
	m_currentYawSpeed = 0.f;
	m_currentPitchSpeed = 0.f;
	m_currentRollSpeed = 0.f;
	m_lastOrientation = FRotator::ZeroRotator.Quaternion();
}

UFireflyMovementComponent::~UFireflyMovementComponent() {
}

void UFireflyMovementComponent::SetMovementOwner(APawn* owner) {
	PawnOwner = owner;
}

void UFireflyMovementComponent::InitializeComponent() {
	Super::InitializeComponent();
	m_capsule = Cast<UCapsuleComponent>(UpdatedComponent);
}

void UFireflyMovementComponent::SetPlanet(APlanetActor const* planet) {
	m_planet = planet;
	m_lastNormal = (FQuat::FindBetween(m_capsule->GetUpVector(), -m_planet->GetGravityDirection(m_capsule->GetComponentLocation())).Rotator());
}

void UFireflyMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	// Move plan forwards (with sweep so we stop when we collide with things).
	m_capsule->AddLocalOffset(FVector(m_currentForwardSpeed * deltaTime, 0.f, 0.f), true);

	// Calculate change in rotation this frame.
	FRotator deltaRotation(0, 0, 0);
	deltaRotation.Pitch = m_currentPitchSpeed * deltaTime;
	deltaRotation.Yaw = m_currentYawSpeed * deltaTime;
	deltaRotation.Roll = m_currentRollSpeed * deltaTime;
	m_lastOrientation = deltaRotation.Quaternion() * m_lastOrientation;
	m_capsule->SetWorldRotation(deltaRotation);

	// Calculate the orientation taking the gravity in account.
	FVector gravity = -m_planet->GetGravityDirection(m_capsule->GetComponentLocation());
	const FQuat deltaQuat = FQuat::FindBetween(m_capsule->GetUpVector(), -m_planet->GetGravityDirection(m_capsule->GetComponentLocation()));
	const FQuat targetQuat = deltaQuat * m_capsule->GetComponentRotation().Quaternion();
	m_lastNormal = targetQuat.Rotator();
	m_capsule->SetWorldRotation(m_lastNormal.Quaternion());

	// Calculate new acceleration and speed.
	float currentAcc = m_bSpeeding ? Acceleration : -Deceleration;
	float newForwardSpeed = m_currentForwardSpeed + (GetWorld()->GetDeltaSeconds() * currentAcc);
	m_currentForwardSpeed = FMath::Clamp(newForwardSpeed, 0.f, MaxSpeed);
}

void UFireflyMovementComponent::SpeedUp(bool bSpeedUp) {
	m_bSpeeding = bSpeedUp;
}

void UFireflyMovementComponent::MoveUp(float value) {
	float targetPitchSpeed = value * TurnSpeed * -1.f;
	targetPitchSpeed += FMath::Abs(m_currentYawSpeed) * -0.2f;

	// Smoothly interpolate to target pitch speed.
	m_currentPitchSpeed = FMath::FInterpTo(m_currentPitchSpeed, targetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void UFireflyMovementComponent::MoveRight(float value) {
	// Target yaw speed is based on input.
	float targetYawSpeed = (value * TurnSpeed);

	// Smoothly interpolate to target yaw speed.
	m_currentYawSpeed = FMath::FInterpTo(m_currentYawSpeed, targetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input ?
	const bool bIsTurning = FMath::Abs(value) > 0.2f;

	// If turning, yaw value is used to influence roll.
	// If not turning, roll to reverse current roll value.
	float targetRollSpeed = bIsTurning ? (m_currentYawSpeed * 0.5f) : (m_lastOrientation.Rotator().Roll * -2.f);
	//float targetRollSpeed = bIsTurning ? (m_currentYawSpeed * 0.5f) : (m_capsule->GetComponentRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	m_currentRollSpeed = FMath::FInterpTo(m_currentRollSpeed, targetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}