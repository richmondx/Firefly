// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyPawn.h"
#include "FireflyMeshComponent.h"
#include "FireflyMovementComponent.h"
#include "CollisionSphere.h"

AFireflyPawn::AFireflyPawn() {
	// Create the sphere collision component of the firefly.
	m_sphere = CreateDefaultSubobject<UCollisionSphere>(TEXT("CollisionSphere0"));
	RootComponent = m_sphere;

	// Create the movement component.
	m_movement = CreateDefaultSubobject<UFireflyMovementComponent>(TEXT("FireflyMovementComponent0"));
	m_movement->SetMovementOwner(this);
	m_movement->SetUpdatedComponent(m_sphere);

	// Create the mesh component.
	m_mesh = CreateDefaultSubobject<UFireflyMeshComponent>(TEXT("FireflyMeshComponent0"));
	m_mesh->SetCastShadow(false);
	m_mesh->SetCollisionProfileName(TEXT("FireflyMesh"));
	m_mesh->bGenerateOverlapEvents = false;
	m_mesh->SetNotifyRigidBodyCollision(false);
	m_mesh->AttachTo(m_sphere);

	// Create a spring arm component.
	m_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	m_springArm->AttachTo(m_sphere);
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
	m_camera->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	m_camera->FieldOfView = 70.f;
	m_camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Create the point light.
	m_light = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight0"));
	m_light->AttachTo(RootComponent);
	m_light->SetIntensity(20000.f);
	m_light->SetLightColor(FLinearColor(1.f, 0.165f, 0.004f));

	// Set initial size.
	SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));

	// Call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

AFireflyPawn::~AFireflyPawn() {
}

void AFireflyPawn::Tick(float deltaSeconds) {
	// Call any parent class Tick implementation.
	Super::Tick(deltaSeconds);
}

void AFireflyPawn::NotifyHit(class UPrimitiveComponent* myComp, class AActor* other, class UPrimitiveComponent* otherComp, bool bSelfMoved, FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hit) {
	UE_LOG(LogTemp, Warning, TEXT("Boom"));
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