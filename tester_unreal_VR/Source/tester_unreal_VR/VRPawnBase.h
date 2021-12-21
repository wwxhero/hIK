// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "tester_unreal_VR.h"
#include "VRPawnBase.generated.h"

UENUM()
enum TRACKER_ID
{
	RH = 0		UMETA(DisplayName = "RightHand"),
	LH 			UMETA(DisplayName = "LeftHand"),
	RF 			UMETA(DisplayName = "RightFoot"),
	LF 			UMETA(DisplayName = "LeftFoot"),
	PW 			UMETA(DisplayName = "Pelvis"),
	Total,
	Unknown = Total
};


UCLASS()
class TESTER_UNREAL_VR_API AVRPawnBase : public APawn
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	TArray<USceneComponent*> Trackers_;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Sort Trackers WRT IDs", CompactNodeTitle = "Identify trackers", Keywords = "Identify trackers"), Category = Settings)
	bool Proc_SortTrackers(const FTransform& hmd2world);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Verify Trackers", CompactNodeTitle = "Verify trackers", Keywords = "Verify trackers"), Category = Settings)
	void VerifyTracker();

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

	void DBG_VisTransform(const FTransform& tm_l2w, float axis_len, float thickness) const;

private:
	TRACKER_ID m_verifying;

};
