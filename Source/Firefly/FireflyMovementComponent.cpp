// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyMovementComponent.h"
#include "FireflyPawn.h"
#include "GravityManager.h"

UFireflyMovementComponent::UFireflyMovementComponent() {
	// Set actors and components.
	m_sphere = nullptr;

	// Set handling parameters.
	Acceleration = 2.f;
	Deceleration = 2.f;

	MMaxSpeed = 700.f;
	MMinSpeed = 200.f;
	MaxSpeed = 400.f;
	Variation = 50.f;
	TurnSpeed = 20.f;
	m_bSpeeding = false;

	m_currentSpeed = 0.f;
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

void UFireflyMovementComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction *thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	// Make the pawn slide along the surfaces.
	FVector move = m_sphere->GetForwardVector() * deltaTime * m_currentSpeed;
	if (!move.IsNearlyZero()) {
		FHitResult hit;
		SafeMoveUpdatedComponent(move, m_sphere->GetComponentRotation(), true, hit);

		if (hit.IsValidBlockingHit()) {
			SlideAlongSurface(move, 1.f - hit.Time, hit.Normal, hit);
		}
	}

	// Calculate change in rotation this frame.
	AGravityManager::RotateComponentAlongGravityDirection(m_sphere);

	m_orientation.Pitch += m_currentPitchSpeed * deltaTime;
	m_orientation.Yaw = m_currentYawSpeed * deltaTime;
	//m_orientation.Roll = m_currentRollSpeed * deltaTime;
	m_sphere->AddLocalRotation(m_orientation);

	// Calculate new acceleration and speed.
	float newSpeed;
	
	if (m_bSpeeding) {
		if (m_currentSpeed < 50.f) {
			m_currentSpeed = 50.f;
		}

		if (m_currentSpeed < MaxSpeed / 3.f) {
			newSpeed = m_currentSpeed * (1.f + GetWorld()->GetDeltaSeconds() * Acceleration);
		} else if (m_currentSpeed < MaxSpeed) {
			newSpeed = m_currentSpeed * (1.f + GetWorld()->GetDeltaSeconds() * 3.f / 2.f * Acceleration * (1.f - m_currentSpeed / MaxSpeed));
		} else {
			newSpeed = FMath::FInterpTo(m_currentSpeed, MaxSpeed, GetWorld()->GetDeltaSeconds(), 0.3f);
		}

	} else {
		if (m_currentSpeed < 50.f) {
			newSpeed = 0.f;
		} else {
			newSpeed = m_currentSpeed * (1.f - GetWorld()->GetDeltaSeconds() * Deceleration);
		}
	}

	m_currentSpeed = newSpeed;
}

void UFireflyMovementComponent::SpeedUp(bool bSpeedUp) {
	m_bSpeeding = bSpeedUp;
}

void UFireflyMovementComponent::Boost() {
	if (MaxSpeed < MMaxSpeed) {
		MaxSpeed = FMath::Min(MaxSpeed + Variation, MMaxSpeed);
		m_currentSpeed = 1.5f * MaxSpeed;
	}
}

void UFireflyMovementComponent::Brake() {
	if (MaxSpeed > MMinSpeed) {
		MaxSpeed = FMath::Max(MaxSpeed - Variation, MMinSpeed);
		m_currentSpeed = m_currentSpeed * 0.4f;
	}
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

void UFireflyMovementComponent::Reorientate(FRotator const &delta) {
	m_sphere->AddLocalRotation(delta);
}