// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawnBase.generated.h"

UCLASS()
class TESTER_UNREAL_VR_API AVRPawnBase : public APawn
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	TArray<USceneComponent*> Trackers_;
public:
	// Sets default values for this pawn's properties
	AVRPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// #if defined _DEBUG
	void DBG_VisTransform(const FTransform& tm_l2w, float axis_len, float thickness) const;
// #endif

};
