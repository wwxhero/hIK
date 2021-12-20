// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawnBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values
AVRPawnBase::AVRPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRPawnBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVRPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto tracker_i : Trackers_)
	{
		DBG_VisTransform(tracker_i->GetComponentTransform(), 10, 1);
	}
}

// Called to bind functionality to input
void AVRPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// #if defined _DEBUG

void AVRPawnBase::DBG_VisTransform(const FTransform& tm_l2w, float axis_len, float thickness) const
{
	const FVector4 ori(0, 0, 0, 1);

	const FVector4 axis_ends[] = {
		FVector4(axis_len,	0,	0,	1),
		FVector4(0,	axis_len,	0,	1),
		FVector4(0,	0,	axis_len,	1),
	};
	const FColor axis_color[] = {
		FColor::Red,
		FColor::Green,
		FColor::Blue
	};
	FVector4 ori_w = tm_l2w.TransformFVector4(ori);
	const int n_axis = sizeof(axis_ends) / sizeof(FVector4);
	for (int i_end = 0; i_end < n_axis; i_end ++)
	{
		FVector4 end_w = tm_l2w.TransformFVector4(axis_ends[i_end]);
		UWorld *world = NULL;
		DrawDebugLine(GetWorld()
					, ori_w
		 			, end_w
		 			, axis_color[i_end]
		 			, false // bPersistentLines =
		 			, -1.f  // LifeTime =
		 			, 0
		 			, thickness);
	}
}


// #endif