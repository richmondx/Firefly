// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "FireflyGameMode.h"
#include "FireflyPawn.h"

AFireflyGameMode::AFireflyGameMode() {
	// Set the default pawn class to FireflyPawn.
	static ConstructorHelpers::FObjectFinder<UClass> Blueprint(TEXT("Class'/Game/blueprints/FireflyPawn_BP.FireflyPawn_BP_C'"));
	if (Blueprint.Object) {
		DefaultPawnClass = Blueprint.Object;
	} else {
		DefaultPawnClass = AFireflyPawn::StaticClass();
	}
}