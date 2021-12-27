// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawnLogRec.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CoreGlobals.h"

static int s_logid = 0;

// Sets default values
AVRPawnLogRec::AVRPawnLogRec()
	: m_hLog(H_INVALID)
	, c_frameID0(GFrameNumber)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_hLog = create_vrlog();
}

AVRPawnLogRec::~AVRPawnLogRec()
{
	if (VALID_HANDLE(m_hLog))
	{
		FString	logFile = FString::Printf(TEXT("vr_%d.log"), s_logid++);
		FString rootDir = FPaths::ProjectDir();
		FString filePathFull = rootDir + logFile;
		save_vrlog_w(m_hLog, *filePathFull);
		release_vrlog(m_hLog);
	}
	m_hLog = H_INVALID;
}

void AVRPawnLogRec::OnVRMsg(TRACKER_ID tracker_id, VR_EVT vrEvt)
{
	// UE_LOG(TESTER_UNREAL_VR, Display, TEXT("AVRPawnLogRec::Proc_VRMsg"));
	// UE_LOG(TESTER_UNREAL_VR, Display, TEXT("Couter: %d, Number: %d"), GFrameCounter, GFrameNumber);
	HLOG_i log_i = getlog_i(m_hLog, GFrameNumber - c_frameID0);
	auto tracker_i = m_trackers[tracker_id];
	check(NULL != tracker_i);
	_TRANSFORM tm_l2p;
	Convert(tracker_i->GetRelativeTransform(), tm_l2p);
	VRMSG msg = { tracker_id, vrEvt, tm_l2p };
	appenditem(log_i, &msg);
	AVRPawnBase::OnVRMsg(tracker_id, vrEvt);
}
