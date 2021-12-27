// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawnLogPB.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values
AVRPawnLogPB::AVRPawnLogPB()
	: m_hLog(H_INVALID)
	, m_frameid0(GFrameNumber)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FString rootDir = FPaths::ProjectDir();
	FString filePathFull = rootDir + FString(TEXT("vr.log"));
	m_hLog = load_vrlog_w(*filePathFull);
}

AVRPawnLogPB::~AVRPawnLogPB()
{
	if (VALID_HANDLE(m_hLog))
		release_vrlog(m_hLog);
	m_hLog = H_INVALID;
}

void AVRPawnLogPB::Tick(float DeltaTime)
{
	if (VALID_HANDLE(m_hLog))
	{
		uint32 logframe_id = GFrameNumber - m_frameid0;
		unsigned int n_logframes = num_framelogs(m_hLog);
		if (logframe_id < n_logframes)
		{
			HLOG_i log_i = getlog_i(m_hLog, logframe_id);
			unsigned int n_items = num_items(log_i);
			TArray<const VRMSG*> delayProc;
			delayProc.SetNum(n_items);
			int n_itemsProc = 0;
			for (unsigned int i_item = 0; i_item < n_items; i_item++)
			{
				const VRMSG* msg_i = get_item(log_i, i_item);
				FTransform tm_i;
				Convert(msg_i->tm, tm_i);
				m_trackers[msg_i->id_tracker]->SetRelativeTransform(tm_i);
				if (VR_EVT::NONE != msg_i->evt)
					delayProc[n_itemsProc++] = msg_i;
			}

			for (int i_itemProc = 0; i_itemProc < n_itemsProc; i_itemProc++)
			{
				auto& msg_i = delayProc[i_itemProc];
				Proc_VRMsg((TRACKER_ID)msg_i->id_tracker, (VR_EVT)msg_i->evt);
			}
		}
		else
		{
			UE_LOG(TESTER_UNREAL_VR, Display, TEXT("PlayBack DONE"));
		}
	}
	
}