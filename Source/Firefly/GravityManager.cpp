// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "GravityManager.h"
#include "PlanetActor.h"
#include "Components/SceneComponent.h"

AGravityManager::AGravityManager() {
}

AGravityManager::~AGravityManager() {
}

void AGravityManager::RotateActorAlongGravityDirection(AActor* target) {
	const FQuat deltaQuat = FQuat::FindBetween(target->GetActorUpVector(), -APlanetActor::GetGravity(target->GetActorLocation()));
	const FQuat targetQuat = deltaQuat * target->GetActorRotation().Quaternion();
	target->SetActorRotation(targetQuat);
}

void AGravityManager::RotateComponentAlongGravityDirection(USceneComponent* target) {
	const FQuat deltaQuat = FQuat::FindBetween(target->GetUpVector(), -APlanetActor::GetGravity(target->GetComponentLocation()));
	const FQuat targetQuat = deltaQuat * target->GetComponentRotation().Quaternion();
	target->SetWorldRotation(targetQuat);
}