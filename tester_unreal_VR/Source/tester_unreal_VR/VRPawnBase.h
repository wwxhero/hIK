// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "tester_unreal_VR.h"
#include "Animation/SkeletalMeshActor.h"
#include "AnimInstance_HIKDrivee.h"
#include "WidgetsVRFSA.h"
#include "SceneCapture2DActorVRSpec.h"
#include "VRPawnBase.generated.h"

UENUM(BlueprintType)
enum VR_EVT
{
	NONE,

	GRIP_PRESS,
	GRIP_RELEASE,
	
	TRIGGER_PRESS,
	TRIGGER_RELEASE,

	TOUCHPAD_UP_PRESS,
	TOUCHPAD_UP_RELEASE,

	TOUCHPAD_DOWN_PRESS,
	TOUCHPAD_DOWN_RELEASE,
};

class AVRPawnBase;

class VRCaliFSA
{
public:
	enum State
	{
		MapLoaded = 0,
		Landed,
		CameraOn,
		IKConnected,
		Exit,
		Any
	};

	class Transition
	{
	public:
		typedef bool (AVRPawnBase::*OnVRMsg_X)();
		Transition(AVRPawnBase& refPawn, State from, State to, TRACKER_ID tid, VR_EVT vrEvt, OnVRMsg_X act)
			: m_refPawn(refPawn)
			, c_from(from)
			, c_to(to)
			, c_tid(tid)
			, c_evt(vrEvt)
			, c_act(act)
		{
		}
	
		bool Execute(State from, TRACKER_ID tid, VR_EVT vrEvt, State& to)
		{
			bool executed = false;
			if ( (c_from == from || c_from == Any)
				&& c_tid == tid
				&& c_evt == vrEvt)
			{
				executed = ((m_refPawn.*c_act)());
				if (executed)
				{
					to = c_to;
				}
			}
			return executed;
		}
	private:
		AVRPawnBase& m_refPawn;
		const State c_from;
		const State c_to;
		const TRACKER_ID c_tid;
		const VR_EVT c_evt;
		const OnVRMsg_X c_act;
	};

	VRCaliFSA();
	~VRCaliFSA();
	void Initialize(AVRPawnBase& refPawn);
	void UpdateState(TRACKER_ID tracker_id, VR_EVT vrEvt);
	State actState() const
	{
		return m_actState;
	}
private:
	State m_actState;
	TArray<FString> m_stateStr;
	TArray<Transition*> m_transitions;
	AVRPawnBase* m_refPawn;
};

UCLASS()
class TESTER_UNREAL_VR_API AVRPawnBase : public APawn

{
	friend class VRCaliFSA;

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
	void Proc_VRMsg(TRACKER_ID tracker_id, VR_EVT evt);

public:
	// Sets default values for this pawn's properties
	AVRPawnBase();

	bool actFloorCali_L();
	bool actFloorCali_R();
	bool actCameraCali_L();
	bool actCameraCali_R();
	bool actCameraCloser();
	bool actCameraFarther();
	bool actConnectIK();
	bool actDisConnectIK();
	bool actQuit();
private:
	void FloorCali_x(const FVector& p_v);
	void VerifyTracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

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

protected:
	ASceneCapture2DActorVRSpec* m_spector;
private:
	TSharedPtr<SWindowVRFSA> m_winVRFSA;
	VRCaliFSA m_caliFSA;
	TRACKER_ID m_verifying;
};
