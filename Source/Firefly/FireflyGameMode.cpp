// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyGameMode.h"
#include "FireflyPawn.h"

AFireflyGameMode::AFireflyGameMode() {
	// Set the default pawn class to FireflyPawn.
	static ConstructorHelpers::FObjectFinder<UBlueprint> Blueprint(TEXT("Blueprint'/Game/blueprints/MyFireflyPawn_BP.MyFireflyPawn_BP'"));
	if (Blueprint.Object) {
		DefaultPawnClass = (UClass*)Blueprint.Object->GeneratedClass;
	} else {
		DefaultPawnClass = AFireflyPawn::StaticClass();
	}
}