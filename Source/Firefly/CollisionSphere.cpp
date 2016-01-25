// Fill out your copyright notice in the Description page of Project Settings.

#include "Firefly.h"
#include "CollisionSphere.h"

UCollisionSphere::UCollisionSphere() {
	InitSphereRadius(100.f);
	SetCollisionProfileName(TEXT("BlockAllDynamic"));
	SetSimulatePhysics(false);
	SetEnableGravity(false);
	SetLinearDamping(0.01f);
	SetAngularDamping(0.f);
	SetNotifyRigidBodyCollision(true);
}

UCollisionSphere::~UCollisionSphere() {
}

#if 0
bool UCollisionSphere::MoveComponentImpl(const FVector& delta, const FQuat& newRotationQuat, bool bSweep, FHitResult* outHit, EMoveComponentFlags moveFlags, ETeleportType teleport) {
	//return Super::MoveComponentImpl(delta, newRotationQuat, bSweep, outHit, moveFlags, teleport);
	SCOPE_CYCLE_COUNTER(STAT_MoveComponentTime);

	ConditionalUpdateComponentToWorld();

	// Find hitResult.
	FHitResult blockingHit(1.f);
	const FVector traceStart = GetComponentLocation();
	const FVector traceEnd = traceStart + delta;
	blockingHit.TraceStart = traceStart;
	blockingHit.TraceEnd = traceEnd;

	// Set up.
	float deltaSizeSq = delta.SizeSquared();
	const FQuat initialRotationQuat = ComponentToWorld.GetRotation();

	// ComponentSweepMulti does nothing if moving < KINDA_SMALL_NUMBER in distance, so it's important to not try to sweep distances smaller than that. 
	const float minMovementDistSq = (bSweep ? FMath::Square(4.f * KINDA_SMALL_NUMBER) : 0.f);
	if (deltaSizeSq <= minMovementDistSq) {
		// Skip if no vector or rotation.
		if (newRotationQuat.Equals(initialRotationQuat, SCENECOMPONENT_QUAT_TOLERANCE)) {
			// copy to optional output param
			if (outHit) {
				*outHit = blockingHit;
			}
			return true;
		}
		deltaSizeSq = 0.f;
	}

	const bool bSkipPhysicsMove = ((moveFlags & MOVECOMP_SkipPhysicsMove) != MOVECOMP_NoFlags);

	bool bMoved = false;
	bool bIncludesOverlapsAtEnd = false;
	bool bRotationOnly = false;
	TArray<FOverlapInfo> pendingOverlaps;
	AActor* const actor = GetOwner();

	if (!bSweep) {
		// not sweeping, just go directly to the new transform
		bMoved = InternalSetWorldLocationAndRotation(traceEnd, newRotationQuat, bSkipPhysicsMove, teleport);
		bRotationOnly = (deltaSizeSq == 0);
		bIncludesOverlapsAtEnd = bRotationOnly && (AreSymmetricRotations(initialRotationQuat, newRotationQuat, GetComponentScale())) && IsCollisionEnabled();

	} else {
		TArray<FHitResult> hits;
		FVector newLocation = traceStart;

		// Perform movement collision checking if needed for this actor.
		const bool bCollisionEnabled = IsCollisionEnabled();
		if (bCollisionEnabled && (deltaSizeSq > 0.f)) {
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) 
			if (!IsRegistered()) {
				if (actor) {
					UE_LOG(LogPrimitiveComponent, Fatal, TEXT("%s MovedComponent %s not initialized deleteme %d"), *actor->GetName(), *GetName(), actor->IsPendingKill());
				
				} else {
					UE_LOG(LogPrimitiveComponent, Fatal, TEXT("MovedComponent %s not initialized"), *GetFullName());
				}
			}
#endif

			UWorld* const world = GetWorld();

			FComponentQueryParams params(TEXT("MoveComponent"), actor);
			FCollisionResponseParams responseParam;
			InitSweepCollisionParams(params, responseParam);
			bool const bHadBlockingHit = world->ComponentSweepMulti(hits, this, traceStart, traceEnd, initialRotationQuat, params);

			if (Hits.Num() > 0)
			{
				const float DeltaSize = FMath::Sqrt(DeltaSizeSq);
				for (int32 HitIdx = 0; HitIdx<Hits.Num(); HitIdx++)
				{
					PullBackHit(Hits[HitIdx], TraceStart, TraceEnd, DeltaSize);
				}
			}

			// If we had a valid blocking hit, store it.
			// If we are looking for overlaps, store those as well.
			uint32 FirstNonInitialOverlapIdx = INDEX_NONE;
			if (bHadBlockingHit || bGenerateOverlapEvents)
			{
				int32 BlockingHitIndex = INDEX_NONE;
				float BlockingHitNormalDotDelta = BIG_NUMBER;
				for (int32 HitIdx = 0; HitIdx < Hits.Num(); HitIdx++)
				{
					const FHitResult& TestHit = Hits[HitIdx];

					if (TestHit.bBlockingHit)
					{
						if (!ShouldIgnoreHitResult(MyWorld, TestHit, Delta, Actor, MoveFlags))
						{
							if (TestHit.Time == 0.f)
							{
								// We may have multiple initial hits, and want to choose the one with the normal most opposed to our movement.
								const float NormalDotDelta = (TestHit.ImpactNormal | Delta);
								if (NormalDotDelta < BlockingHitNormalDotDelta)
								{
									BlockingHitNormalDotDelta = NormalDotDelta;
									BlockingHitIndex = HitIdx;
								}
							}
							else if (BlockingHitIndex == INDEX_NONE)
							{
								// First non-overlapping blocking hit should be used, if an overlapping hit was not.
								// This should be the only non-overlapping blocking hit, and last in the results.
								BlockingHitIndex = HitIdx;
								break;
							}
						}
					}
					else if (bGenerateOverlapEvents)
					{
						UPrimitiveComponent* OverlapComponent = TestHit.Component.Get();
						if (OverlapComponent && OverlapComponent->bGenerateOverlapEvents)
						{
							if (!ShouldIgnoreOverlapResult(MyWorld, Actor, *this, TestHit.GetActor(), *OverlapComponent))
							{
								// don't process touch events after initial blocking hits
								if (BlockingHitIndex >= 0 && TestHit.Time > Hits[BlockingHitIndex].Time)
								{
									break;
								}

								if (FirstNonInitialOverlapIdx == INDEX_NONE && TestHit.Time > 0.f)
								{
									// We are about to add the first non-initial overlap.
									FirstNonInitialOverlapIdx = PendingOverlaps.Num();
								}

								// cache touches
								PendingOverlaps.AddUnique(FOverlapInfo(TestHit));
							}
						}
					}
				}

				// Update blocking hit, if there was a valid one.
				if (BlockingHitIndex >= 0)
				{
					BlockingHit = Hits[BlockingHitIndex];
				}
			}

			// Update NewLocation based on the hit result
			if (!BlockingHit.bBlockingHit)
			{
				NewLocation = TraceEnd;
			}
			else
			{
				NewLocation = TraceStart + (BlockingHit.Time * (TraceEnd - TraceStart));

				// Sanity check
				const FVector ToNewLocation = (NewLocation - TraceStart);
				if (ToNewLocation.SizeSquared() <= MinMovementDistSq)
				{
					// We don't want really small movements to put us on or inside a surface.
					NewLocation = TraceStart;
					BlockingHit.Time = 0.f;

					// Remove any pending overlaps after this point, we are not going as far as we swept.
					if (FirstNonInitialOverlapIdx != INDEX_NONE)
					{
						PendingOverlaps.SetNum(FirstNonInitialOverlapIdx);
					}
				}
			}

			bIncludesOverlapsAtEnd = AreSymmetricRotations(InitialRotationQuat, NewRotationQuat, GetComponentScale());

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if ((BlockingHit.Time < 1.f) && !IsZeroExtent())
			{
				// this is sole debug purpose to find how capsule trace information was when hit 
				// to resolve stuck or improve our movement system - To turn this on, use DebugCapsuleSweepPawn
				APawn const* const ActorPawn = (Actor ? Cast<APawn>(Actor) : NULL);
				if (ActorPawn && ActorPawn->Controller && ActorPawn->Controller->IsLocalPlayerController())
				{
					APlayerController const* const PC = CastChecked<APlayerController>(ActorPawn->Controller);
					if (PC->CheatManager && PC->CheatManager->bDebugCapsuleSweepPawn)
					{
						FVector CylExtent = ActorPawn->GetSimpleCollisionCylinderExtent()*FVector(1.001f, 1.001f, 1.0f);
						FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CylExtent);
						PC->CheatManager->AddCapsuleSweepDebugInfo(TraceStart, TraceEnd, BlockingHit.ImpactPoint, BlockingHit.Normal, BlockingHit.ImpactNormal, BlockingHit.Location, CapsuleShape.GetCapsuleHalfHeight(), CapsuleShape.GetCapsuleRadius(), true, (BlockingHit.bStartPenetrating && BlockingHit.bBlockingHit) ? true : false);
					}
				}
			}
#endif
		}
		else if (DeltaSizeSq > 0.f)
		{
			// apply move delta even if components has collisions disabled
			NewLocation += Delta;
			bIncludesOverlapsAtEnd = false;
		}
		else if (DeltaSizeSq == 0.f && bCollisionEnabled)
		{
			bIncludesOverlapsAtEnd = AreSymmetricRotations(InitialRotationQuat, NewRotationQuat, GetComponentScale());
			bRotationOnly = true;
		}

		// Update the location.  This will teleport any child components as well (not sweep).
		bMoved = InternalSetWorldLocationAndRotation(NewLocation, NewRotationQuat, bSkipPhysicsMove, Teleport);
	}

	// Handle overlap notifications.
	if (bMoved)
	{
		if (IsDeferringMovementUpdates())
		{
			// Defer UpdateOverlaps until the scoped move ends.
			FScopedMovementUpdate* ScopedUpdate = GetCurrentScopedMovement();
			if (bRotationOnly && bIncludesOverlapsAtEnd)
			{
				ScopedUpdate->KeepCurrentOverlapsAfterRotation(bSweep);
			}
			else
			{
				ScopedUpdate->AppendOverlapsAfterMove(PendingOverlaps, bSweep, bIncludesOverlapsAtEnd);
			}
		}
		else
		{
			if (bIncludesOverlapsAtEnd)
			{
				TArray<FOverlapInfo> OverlapsAtEndLocation;
				const TArray<FOverlapInfo>* OverlapsAtEndLocationPtr = nullptr; // When non-null, used as optimization to avoid work in UpdateOverlaps.
				if (bRotationOnly)
				{
					OverlapsAtEndLocationPtr = ConvertRotationOverlapsToCurrentOverlaps(OverlapsAtEndLocation, GetOverlapInfos());
				}
				else
				{
					OverlapsAtEndLocationPtr = ConvertSweptOverlapsToCurrentOverlaps(OverlapsAtEndLocation, PendingOverlaps, 0, GetComponentLocation(), GetComponentQuat());
				}
				UpdateOverlaps(&PendingOverlaps, true, OverlapsAtEndLocationPtr);
			}
			else
			{
				UpdateOverlaps(&PendingOverlaps, true, nullptr);
			}
		}
	}

	// Handle blocking hit notifications. Avoid if pending kill (which could happen after overlaps).
	if (BlockingHit.bBlockingHit && !IsPendingKill())
	{
		if (IsDeferringMovementUpdates())
		{
			FScopedMovementUpdate* ScopedUpdate = GetCurrentScopedMovement();
			ScopedUpdate->AppendBlockingHitAfterMove(BlockingHit);
		}
		else
		{
			DispatchBlockingHit(*Actor, BlockingHit);
		}
	}

#if defined(PERF_SHOW_MOVECOMPONENT_TAKING_LONG_TIME) || LOOKING_FOR_PERF_ISSUES
	UNCLOCK_CYCLES(MoveCompTakingLongTime);
	const float MSec = FPlatformTime::ToMilliseconds(MoveCompTakingLongTime);
	if (MSec > PERF_SHOW_MOVECOMPONENT_TAKING_LONG_TIME_AMOUNT)
	{
		if (GetOwner())
		{
			UE_LOG(LogPrimitiveComponent, Log, TEXT("%10f executing MoveComponent for %s owned by %s"), MSec, *GetName(), *GetOwner()->GetFullName());
		}
		else
		{
			UE_LOG(LogPrimitiveComponent, Log, TEXT("%10f executing MoveComponent for %s"), MSec, *GetFullName());
		}
	}
#endif

	// copy to optional output param
	if (OutHit)
	{
		*OutHit = BlockingHit;
	}

	// Return whether we moved at all.
	return bMoved;
}
#endif