// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "tester_unreal_VR.h"
#include "Animation/SkeletalMeshActor.h"
#include "AnimInstance_HIKDrivee.h"
#include "VRPawnBase.generated.h"

UENUM(BlueprintType)
enum VR_EVT
{
	NONE,

	GRIP_PRESS,
	GRIP_RELEASE,
	
	TRIGGER_PRESS,
	TRIGGER_RELEASE
};


UCLASS()
class TESTER_UNREAL_VR_API AVRPawnBase : public APawn
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool InitVRPawn(USceneComponent* org
					, USceneComponent* hmd
					, USceneComponent* ctrller_l
					, USceneComponent* ctrller_r
					, const TArray<USceneComponent*>& trackers
					, ASkeletalMeshActor* avatarIKDrivee);

	UFUNCTION(BlueprintCallable, Category = Settings)
	void Proc_FloorCali(const FVector& p_v);

	UFUNCTION(BlueprintCallable, Category = Settings)
	bool Proc_SortTrackers();

	UFUNCTION(BlueprintCallable, Category = Settings)
	void Proc_ConnectIKTaget();

	UFUNCTION(BlueprintCallable, Category = Settings)
	void VerifyTracker();

	UFUNCTION(BlueprintCallable, Category = Settings)
	void Proc_VRMsg(TRACKER_ID tracker_id, VR_EVT evt);

public:
	// Sets default values for this pawn's properties
	AVRPawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnVRMsg(TRACKER_ID tracker_id, VR_EVT evt);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DBG_VisTransform(const FTransform& tm_l2w, float axis_len, float thickness) const;

protected:
	USceneComponent* m_vrOrg;
	TArray<USceneComponent*> m_trackers;
	ASkeletalMeshActor* m_actorIKDrivee;
	UAnimInstance_HIKDrivee* m_animIKDrivee;
private:
	TRACKER_ID m_verifying;

};
