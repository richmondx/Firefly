// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyMeshComponent.h"

UFireflyMeshComponent::UFireflyMeshComponent() {
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> mesh;
		FConstructorStatics()
			: mesh(TEXT("/Game/models/Firefly/Firefly"))
		{
		}
	};

	static FConstructorStatics ConstructorStatics;
	SetStaticMesh(ConstructorStatics.mesh.Get());
}

UFireflyMeshComponent::~UFireflyMeshComponent() {
}
