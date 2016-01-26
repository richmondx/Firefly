// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "FireflyMeshComponent.generated.h"

UCLASS()
class FIREFLY_API UFireflyMeshComponent : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	/** Constructor. */
	UFireflyMeshComponent();

	/** Destructor. */
	virtual ~UFireflyMeshComponent();
};
