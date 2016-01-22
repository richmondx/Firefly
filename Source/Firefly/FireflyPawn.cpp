// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyPawn.h"
#include "FireflyMeshComponent.h"
#include "FireflyMovementComponent.h"
#include "PlanetActor.h"

AFireflyPawn::AFireflyPawn() {
	// Create the capsule component of the firefly.
	m_capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent0"));
	m_capsule->InitCapsuleSize(42.0f, 42.0f);
	m_capsule->SetCollisionProfileName(TEXT("Firefly"));
	//m_capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//m_capsule->SetSimulatePhysics(true);
	m_capsule->SetEnableGravity(false);
	m_capsule->GetBodyInstance()->COMNudge = FVector(0.0f, 0.0f, -96.0f);
	m_capsule->SetLinearDamping(0.f);
	m_capsule->SetAngularDamping(0.f);
	m_capsule->SetNotifyRigidBodyCollision(true);
	RootComponent = m_capsule;

	// Create the movement component.
	m_movement = CreateDefaultSubobject<UFireflyMovementComponent>(TEXT("FireflyMovementComponent0"));
	m_movement->SetMovementOwner(this);
	m_movement->SetUpdatedComponent(m_capsule);

	// Create the mesh component.
	m_mesh = CreateDefaultSubobject<UFireflyMeshComponent>(TEXT("FireflyMeshComponent0"));
	m_mesh->CastShadow = false;
	m_mesh->SetCollisionProfileName(TEXT("FireflyMesh"));
	m_mesh->bGenerateOverlapEvents = false;
	m_mesh->SetNotifyRigidBodyCollision(false);
	m_mesh->AttachTo(m_capsule);

	// Create a spring arm component.
	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	m_springArm->AttachTo(m_capsule);
	m_springArm->TargetArmLength = 0.f; // The camera follows at this distance behind the character	
	m_springArm->bDoCollisionTest = false;
	m_springArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	m_springArm->bEnableCameraLag = false;
	m_springArm->CameraLagSpeed = 15.f;
	m_springArm->bInheritPitch = true;
	m_springArm->bInheritYaw = true;
	m_springArm->bInheritRoll = true;

	// Create camera component.
	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	m_camera->AttachTo(m_springArm, USpringArmComponent::SocketName);
	m_camera->SetRelativeLocation(FVector(0.0f, 0.0f, 600.0f));
	m_camera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 3000.0f), FRotator(-90.f, 0.f, 0.f));
	m_camera->FieldOfView = 70.f;
	m_camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

AFireflyPawn::~AFireflyPawn() {
}

void AFireflyPawn::SetPlanet(APlanetActor const *planet) {
	m_movement->SetPlanet(planet);
}

void AFireflyPawn::Tick(float deltaSeconds) {
	// Call any parent class Tick implementation.
	Super::Tick(deltaSeconds);
}

void AFireflyPawn::NotifyHit(class UPrimitiveComponent* myComp, class AActor* other, class UPrimitiveComponent* otherComp, bool bSelfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit) {
	Super::NotifyHit(myComp, other, otherComp, bSelfMoved, hitLocation, hitNormal, normalImpulse, hit);
}

void AFireflyPawn::SetupPlayerInputComponent(class UInputComponent* inputComponent) {
	check(inputComponent);

	// Bind speeding actions.
	inputComponent->BindAction("SpeedUp", IE_Pressed, this, &AFireflyPawn::StartSpeedUp);
	inputComponent->BindAction("SpeedUp", IE_Released, this, &AFireflyPawn::StopSpeedUp);

	// Bind our control axis' to callback functions
	inputComponent->BindAxis("MoveUp", this, &AFireflyPawn::MoveUpInput);
	inputComponent->BindAxis("MoveRight", this, &AFireflyPawn::MoveRightInput);
}

void AFireflyPawn::StartSpeedUp() {
	m_movement->SpeedUp(true);
}

void AFireflyPawn::StopSpeedUp() {
	m_movement->SpeedUp(false);
}

void AFireflyPawn::MoveUpInput(float value) {
	m_movement->MoveUp(value);
}

void AFireflyPawn::MoveRightInput(float value) {
	m_movement->MoveRight(value);
}