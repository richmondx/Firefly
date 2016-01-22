// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyGameMode.h"
#include "FireflyPawn.h"

AFireflyGameMode::AFireflyGameMode() {
	// Set the default pawn class to FireflyPawn.
	DefaultPawnClass = AFireflyPawn::StaticClass();
}

